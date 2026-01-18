/* USER CODE BEGIN Header */
/**
  * @file           : main.c
  * @brief          : STM32F4 + AI-WB-01S (TRANSPARENT MODE + FIRE ALARM)
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart6;
DHTxx_Drv_t dht11;

// Buffer
char rx_buffer_poll[256];
char at_resp_buffer[512];
char tx_buffer[256];

// Biến cảm biến
float temp = 0.0f;
float hum = 0.0f;
int gas_mq2 = 0;
int gas_mq135 = 0;
int motion = 0;

// Trạng thái
bool is_fire_alarm = false;
// Vì dùng Transparent Mode, ta mặc định sau khi Init xong là Connected
bool is_transparent_mode = false;

#define GAS_ALARM_THRESHOLD 2500

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART6_UART_Init(void);
void annouce_buzzer(int duration_ms);

/* ============================================================================== */
/* CORE FUNCTIONS (USER'S LOGIC)                                                  */
/* ============================================================================== */

// 1. Gửi AT và chờ phản hồi (Giữ nguyên logic của bạn)
bool AT_Send_And_Wait(char* cmd, char* target_response, uint32_t timeout) {
    memset(at_resp_buffer, 0, sizeof(at_resp_buffer));
    __HAL_UART_FLUSH_DRREGISTER(&huart6);

    HAL_UART_Transmit(&huart6, (uint8_t*)cmd, strlen(cmd), 100);

    uint32_t tickStart = HAL_GetTick();
    uint16_t idx = 0;

    while ((HAL_GetTick() - tickStart) < timeout) {
        uint8_t ch;
        if (HAL_UART_Receive(&huart6, &ch, 1, 10) == HAL_OK) {
            if (idx < sizeof(at_resp_buffer) - 1) {
                at_resp_buffer[idx++] = ch;
            }
        }
        if (strstr(at_resp_buffer, target_response) != NULL) {
            return true;
        }
    }
    return false;
}

// 2. KẾT NỐI WIFI (DÙNG 100% CODE CỦA BẠN)
void WiFi_Connect_Polling(void) {
    char cmd[128];
    annouce_buzzer(200); // Bíp cái báo bắt đầu

    // AT Check
    while (!AT_Send_And_Wait("AT\r\n", "OK", 1000)) HAL_Delay(500);
    annouce_buzzer(50);

    // Config
    AT_Send_And_Wait("AT+RST\r\n", "ready", 5000);
    HAL_Delay(2000);
    AT_Send_And_Wait("ATE0\r\n", "OK", 1000);
    // Config Station mode
    while (!AT_Send_And_Wait("AT+WMODE=1,1\r\n", "OK", 2000)) HAL_Delay(500);

    // Connect Wifi
    sprintf(cmd, "AT+WJAP=%s,%s\r\n", WIFI_SSID, WIFI_PASSWORD);
    // Chờ đến khi có IP
    while (!AT_Send_And_Wait(cmd, "+EVENT:WIFI_GOT_IP", 20000)) {
        annouce_buzzer(500); // Kêu dài báo lỗi wifi
        HAL_Delay(2000);
        // Retry loop
        AT_Send_And_Wait(cmd, "+EVENT:WIFI_GOT_IP", 1000);
    }
    annouce_buzzer(100); annouce_buzzer(100);

    // Connect Server
    sprintf(cmd, "AT+SOCKET=4,%s,%d\r\n", SERVER_IP, SERVER_PORT);
    while (!AT_Send_And_Wait(cmd, "connect success", 5000)) {
        // Nếu đã connect rồi thì thôi
        if (strstr(at_resp_buffer, "ALREADY CONNECTED")) break;
        HAL_Delay(2000);
    }

    // Transparent Mode (Truyền nhận trong suốt)
    AT_Send_And_Wait("AT+SOCKETTT\r\n", ">", 3000);

    is_transparent_mode = true; // Đánh dấu đã vào chế độ truyền thẳng

    annouce_buzzer(100); annouce_buzzer(100); annouce_buzzer(100);

    // Handshake
    char handshake[32];
    sprintf(handshake, "CONNECT:%s\n", DEVICE_NODE_ID);
    HAL_UART_Transmit(&huart6, (uint8_t*)handshake, strlen(handshake), 1000);
}

// 3. Gửi Data (ĐÃ SỬA: Gửi thẳng, không dùng CIPSEND vì đang ở SOCKETTT)
void WiFi_Send_Raw(char* data) {
    if (is_transparent_mode) {
        HAL_UART_Transmit(&huart6, (uint8_t*)data, strlen(data), 1000);
    }
}

// 4. Điều khiển thiết bị
void Control_Device(char* device, char* value) {
    // --- BẢO VỆ CHÁY NỔ ---
    // Nếu đang cháy, KHÔNG ĐƯỢC tắt Buzzer
    if (is_fire_alarm && strcmp(device, "BUZZER") == 0 &&
       (strcmp(value, "OFF") == 0 || strcmp(value, "0") == 0)) {
        return;
    }

    GPIO_PinState state = GPIO_PIN_RESET;
    // Mạch kích mức CAO: ON = SET
    if (strcmp(value, "ON") == 0 || strcmp(value, "1") == 0) state = GPIO_PIN_SET;
    else state = GPIO_PIN_RESET;

    if (strcmp(device, "RELAY_1") == 0) HAL_GPIO_WritePin(RELAY1_Port, RELAY1_Pin, state);
    else if (strcmp(device, "RELAY_2") == 0) HAL_GPIO_WritePin(RELAY2_Port, RELAY2_Pin, state);
    else if (strcmp(device, "RELAY_3") == 0) HAL_GPIO_WritePin(RELAY3_Port, RELAY3_Pin, state);
    else if (strcmp(device, "BUZZER") == 0) HAL_GPIO_WritePin(BUZZER_TEST_Port, BUZZER_TEST_Pin, state);
}

// 5. Nhận và Xử lý Lệnh (Trong mode SOCKETTT, dữ liệu server gửi về sẽ vào thẳng RX)
void WiFi_Check_Downlink(void) {
    memset(rx_buffer_poll, 0, sizeof(rx_buffer_poll));

    // Polling nhanh buffer
    if (HAL_UART_Receive(&huart6, (uint8_t*)rx_buffer_poll, sizeof(rx_buffer_poll)-1, 10) == HAL_OK) {
        // Tìm chuỗi "CMD:"
        char* cmd_ptr = strstr(rx_buffer_poll, "CMD:");
        if (cmd_ptr != NULL) {
            char device[20] = {0};
            char value[20] = {0};
            // Format: CMD:RELAY_1:ON
            if (sscanf(cmd_ptr, "CMD:%[^:]:%s", device, value) == 2) {
                Control_Device(device, value);
            }
        }
    }
}

// Helper: Buzzer
void annouce_buzzer(int duration_ms) {
    HAL_GPIO_WritePin(BUZZER_TEST_Port, BUZZER_TEST_Pin, GPIO_PIN_SET);
    HAL_Delay(duration_ms);
    HAL_GPIO_WritePin(BUZZER_TEST_Port, BUZZER_TEST_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
}

/* ============================================================================== */
/* MAIN                                                                           */
/* ============================================================================== */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART6_UART_Init();

    // Init Thư viện (Theo đúng code thư viện bạn gửi)
    DwtInit();
    dht11.DataPort = DHT_Port;
    dht11.DataPin = DHT_Pin;
    dht11.Type = DHT11;

    // KẾT NỐI WIFI (Dùng hàm của bạn)
    WiFi_Connect_Polling();

    uint32_t last_uplink = 0;

    while (1) {
        // 1. ĐỌC CẢM BIẾN & CHECK CHÁY
        if (HAL_GetTick() - last_uplink > UPLINK_INTERVAL) {
            DHT_GetData(&dht11);
            temp = dht11.Temperature;
            hum = dht11.Humidity;

            HAL_ADC_Start(&hadc1);
            if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
                gas_mq2 = HAL_ADC_GetValue(&hadc1);
            }
            HAL_ADC_Stop(&hadc1);

            motion = HAL_GPIO_ReadPin(PIR_Port, PIR_Pin);

            // --- LOCAL FIRE ALARM ---
            if (gas_mq2 > GAS_ALARM_THRESHOLD) {
                is_fire_alarm = true;
                HAL_GPIO_WritePin(BUZZER_TEST_Port, BUZZER_TEST_Pin, GPIO_PIN_SET);
            } else {
                if(is_fire_alarm) {
                     is_fire_alarm = false;
                     HAL_GPIO_WritePin(BUZZER_TEST_Port, BUZZER_TEST_Pin, GPIO_PIN_RESET);
                }
            }

            // GỬI JSON (RAW qua SOCKETTT)
            memset(tx_buffer, 0, sizeof(tx_buffer));
            sprintf(tx_buffer,
                "{\"temp\":%.1f,\"hum\":%.1f,\"gas_mq2\":%d,\"gas_mq135\":%d,\"motion\":%d}\n",
                temp, hum, gas_mq2, gas_mq135, motion);

            WiFi_Send_Raw(tx_buffer);

            last_uplink = HAL_GetTick();
        }

        // 2. CHECK LỆNH ĐIỀU KHIỂN
        WiFi_Check_Downlink();

        /* LƯU Ý VỀ RECONNECT:
           Với mode SOCKETTT, nếu mất kết nối, thường ta phải Reset module để chạy lại từ đầu.
           Bạn có thể thêm logic: Nếu gửi data mãi không thấy server phản hồi (Heartbeat) -> Gọi lại WiFi_Connect_Polling().
           Nhưng hiện tại để đơn giản ta cứ chạy vòng lặp này đã.
        */
    }
}

// ... (Giữ nguyên SystemClock_Config và các hàm MX_...)
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) { Error_Handler(); }
}
static void MX_ADC1_Init(void) {
  ADC_ChannelConfTypeDef sConfig = {0};
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK) { Error_Handler(); }
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) { Error_Handler(); }
}
static void MX_USART6_UART_Init(void) {
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK) { Error_Handler(); }
}
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOA, LED_TEST_Pin|BUZZER_TEST_Pin|RELAY1_Pin|RELAY2_Pin|RELAY3_Pin, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = LED_TEST_Pin|BUZZER_TEST_Pin|RELAY1_Pin|RELAY2_Pin|RELAY3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = PIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void Error_Handler(void) {
  __disable_irq();
  while (1) {}
}
