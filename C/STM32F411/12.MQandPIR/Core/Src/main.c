/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Smart Home Node - ROBUST CONNECTION & DOWNLINK PARSER
  * @version        : 5.0 (Final Architecture)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c_lcd.h"
#include "dhtxx.h"
#include "dwt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Để dùng atoi
#include "ring_buffer.h"

/* Config --------------------------------------------------------------------*/
#define DEVICE_NODE_ID  "NODE_01"          // <--- QUAN TRỌNG: ID của mạch này
#define WIFI_SSID       "Herukyatto"
#define WIFI_PASSWORD   "123456789"
#define SERVER_IP       "192.168.137.120"  // IP của Jetson Nano
#define SERVER_PORT     5000
#define SEND_INTERVAL   10000              // 10 giây gửi 1 lần

/* Variables -----------------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart6;

I2C_LCD_HandleTypeDef lcd;
DHTxx_Drv_t dht11;

volatile uint32_t ms_ticks = 0;
RingBuffer wifi_rb;
uint8_t rx_data_temp;

// Buffer xử lý lệnh Downlink
char rx_line_buffer[128];
uint16_t rx_line_index = 0;

// Buffer cho AT Command (Blocking)
char response_buffer[512];
uint16_t response_index = 0;

/* Prototypes ----------------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART6_UART_Init(void);
void Error_Handler(void);

int send_at_command(char* command, char* expected_response, uint32_t timeout);
void WiFi_Connect_Robust(void); // Hàm kết nối mới
void Process_Command(char* cmd_str); // Hàm xử lý lệnh
uint32_t ADC_Read_Channel(uint32_t channel);

/* Main ----------------------------------------------------------------------*/
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_USART6_UART_Init();

  DwtInit();

  // 1. LCD Init (Double Init as requested)
  lcd.hi2c = &hi2c1;
  lcd.address = 0x4E;

  lcd_init(&lcd); // Lần 1
  HAL_Delay(100);
  lcd_init(&lcd); // Lần 2 cho chắc ăn

  lcd_clear(&lcd);
  lcd_puts(&lcd, "System Booting...");
  HAL_Delay(1000);

  // 2. Setup DHT
  dht11.DataPort = DHT22_GPIO_Port;
  dht11.DataPin = DHT22_Pin;
  dht11.Type = DHT11;

  // 3. Kết nối WiFi & Server (Chế độ tin cậy tuyệt đối)
  WiFi_Connect_Robust();

  // 4. Setup Non-blocking cho vòng lặp chính
  RB_Init(&wifi_rb);
  HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1); // Bắt đầu nhận ngắt UART

  if(HAL_TIM_Base_Start_IT(&htim4) != HAL_OK) {
      Error_Handler();
  }

  lcd_clear(&lcd);
  lcd_puts(&lcd, "Ready: ");
  lcd_puts(&lcd, DEVICE_NODE_ID);

  uint32_t last_send_time = 0;

  /* --- LOOP --- */
  while (1)
  {
      // --- A. XỬ LÝ NHẬN LỆNH (DOWNLINK) ---
      // Đọc từng byte từ RingBuffer để ghép thành câu lệnh
      while (!RB_IsEmpty(&wifi_rb))
      {
          uint8_t byte;
          RB_Get(&wifi_rb, &byte);

          // Nếu gặp ký tự xuống dòng (\n) -> Kết thúc 1 lệnh -> Xử lý ngay
          if (byte == '\n' || byte == '\r')
          {
              if (rx_line_index > 0) // Chỉ xử lý nếu buffer có dữ liệu
              {
                  rx_line_buffer[rx_line_index] = 0; // Null-terminate string
                  Process_Command(rx_line_buffer);   // <--- GỌI HÀM XỬ LÝ
                  rx_line_index = 0; // Reset index cho lệnh tiếp theo
              }
          }
          else
          {
              // Gom ký tự vào buffer
              if (rx_line_index < sizeof(rx_line_buffer) - 1) {
                  rx_line_buffer[rx_line_index++] = (char)byte;
              }
          }
      }

      // --- B. GỬI DỮ LIỆU ĐỊNH KỲ (UPLINK) ---
      if (ms_ticks - last_send_time >= SEND_INTERVAL)
      {
          last_send_time = ms_ticks;

          // Đọc cảm biến
          DHT_GetData(&dht11);
          uint32_t mq2_val = ADC_Read_Channel(ADC_CHANNEL_0);
          uint32_t mq135_val = ADC_Read_Channel(ADC_CHANNEL_1);
          uint8_t pir_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);

          // Gửi JSON kèm ID
          char json_msg[128];
          sprintf(json_msg,
                  "{\"id\":\"%s\",\"t\":%.1f,\"h\":%.1f,\"mq2\":%lu,\"mq135\":%lu,\"pir\":%d}\n",
                  DEVICE_NODE_ID,
                  dht11.Temperature,
                  dht11.Humidity,
                  mq2_val,
                  mq135_val,
                  pir_state);

          HAL_UART_Transmit(&huart6, (uint8_t*)json_msg, strlen(json_msg), 100);

          // Chỉ update dòng 1 LCD để không đè lên thông báo lệnh (dòng 2)
          char disp[20];
          lcd_gotoxy(&lcd, 0, 0);
          sprintf(disp, "T:%.0f G:%lu P:%d   ", dht11.Temperature, mq2_val, pir_state);
          lcd_puts(&lcd, disp);
      }
  }
}

/* ============================================================ */
/* HÀM XỬ LÝ LOGIC                                              */
/* ============================================================ */

// Hàm phân tích và thực thi lệnh từ Jetson
// Định dạng lệnh: CMD:<DEVICE>:<VALUE> (Ví dụ: CMD:MQ2:1)
void Process_Command(char* cmd_str)
{
    char type[10];
    int value;

    // Kiểm tra xem có phải lệnh CMD không
    if (sscanf(cmd_str, "CMD:%[^:]:%d", type, &value) == 2)
    {
        // Hiển thị lệnh nhận được lên dòng 2 LCD
        lcd_gotoxy(&lcd, 0, 1);
        char lcd_buf[17];
        sprintf(lcd_buf, "Set %s -> %d  ", type, value); // Thêm khoảng trắng để xóa chữ cũ
        lcd_puts(&lcd, lcd_buf);

        // --- ĐIỀU KHIỂN PHẦN CỨNG ---

        // 1. Nếu là lệnh cho RELAY/LED (Hoặc test bằng MQ2 như bạn đã map)
        // Hiện tại bạn map Device 18 (MQ2) để test nút bấm
        if (strcmp(type, "MQ2") == 0 || strcmp(type, "LED") == 0 || strcmp(type, "RELAY") == 0)
        {
            if (value == 1) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Bật LED PA9
            } else {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); // Tắt LED PA9
            }
        }

        // 2. Nếu là lệnh cho Còi (ALARM/BUZZER)
        else if (strcmp(type, "BUZZER") == 0)
        {
             // Code bật còi ở đây (nếu có chân)
        }
    }
}

// Hàm kết nối WiFi "Lì đòn" - Không xong không đi tiếp
void WiFi_Connect_Robust(void) {
    // Bước 1: Kiểm tra AT (Giao tiếp module)
    lcd_clear(&lcd); lcd_puts(&lcd, "Checking AT...");
    while(1) {
        if (send_at_command("AT\r\n", "OK", 1000)) {
            lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "AT OK!      ");
            HAL_Delay(500);
            break; // Thoát vòng lặp để sang bước sau
        } else {
            lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Retrying... ");
            HAL_Delay(500); // Thử lại sau 0.5s
        }
    }

    // Bước 2: Cấu hình Mode Station
    lcd_clear(&lcd); lcd_puts(&lcd, "Setting Mode...");
    send_at_command("AT+RST\r\n", "ready", 5000);
    HAL_Delay(2000);
    send_at_command("ATE0\r\n", "OK", 1000);

    while(1) {
        if(send_at_command("AT+WMODE=1,1\r\n", "OK", 2000)) {
            break;
        }
        lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Retrying... ");
        HAL_Delay(500);
    }

    // Bước 3: Kết nối WiFi
    lcd_clear(&lcd); lcd_puts(&lcd, "Connecting WiFi");
    char connect_cmd[128];
    sprintf(connect_cmd, "AT+WJAP=%s,%s\r\n", WIFI_SSID, WIFI_PASSWORD);

    while(1) {
        // Thử kết nối, timeout dài (20s) để chờ WiFi
        if (send_at_command(connect_cmd, "+EVENT:WIFI_GOT_IP", 20000)) {
            lcd_clear(&lcd); lcd_puts(&lcd, "WiFi Connected!");
            HAL_Delay(1000);
            break;
        } else {
            lcd_clear(&lcd); lcd_puts(&lcd, "WiFi Failed!");
            lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Retrying... ");
            HAL_Delay(2000); // Đợi 2s rồi thử lại
        }
    }

    // Bước 4: Kết nối Server (Jetson)
    lcd_clear(&lcd); lcd_puts(&lcd, "Link Server...");
    char sock_cmd[128];
    sprintf(sock_cmd, "AT+SOCKET=4,%s,%d\r\n", SERVER_IP, SERVER_PORT);

    while(1) {
        if(send_at_command(sock_cmd, "connect success", 5000)) {
             lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Socket OK!  ");
             HAL_Delay(1000);
             break;
        } else {
             lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Retrying... ");
             HAL_Delay(2000);
        }
    }

    // Bước 5: Transparent Mode
    lcd_clear(&lcd); lcd_puts(&lcd, "Transparent...");
    send_at_command("AT+SOCKETTT\r\n", ">", 3000);
    HAL_Delay(500);
}

// Các hàm Helper giữ nguyên
int send_at_command(char* command, char* expected_response, uint32_t timeout) {
    memset(response_buffer, 0, sizeof(response_buffer));
    response_index = 0;
    HAL_UART_Transmit(&huart6, (uint8_t*)command, strlen(command), 100);
    uint32_t start_time = HAL_GetTick();
    while (HAL_GetTick() - start_time < timeout) {
        uint8_t byte;
        if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE)) {
            HAL_UART_Receive(&huart6, &byte, 1, 10);
            if (response_index < sizeof(response_buffer) - 1) {
                response_buffer[response_index++] = byte;
                response_buffer[response_index] = 0;
                if (strstr(response_buffer, expected_response) != NULL) return 1;
            }
        }
    }
    return 0;
}

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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

static void MX_ADC1_Init(void) {
  __HAL_RCC_ADC1_CLK_ENABLE();
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
  HAL_ADC_Init(&hadc1);
}

static void MX_I2C1_Init(void) {
  __HAL_RCC_I2C1_CLK_ENABLE();
  hi2c1.Instance = I2C1; hi2c1.Init.ClockSpeed = 100000; hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; hi2c1.Init.OwnAddress1 = 0; hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; hi2c1.Init.OwnAddress2 = 0; hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
}

static void MX_TIM4_Init(void) {
  __HAL_RCC_TIM4_CLK_ENABLE();
  HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  TIM_ClockConfigTypeDef sClockSourceConfig = {0}; TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim4.Instance = TIM4; htim4.Init.Prescaler = 99; htim4.Init.CounterMode = TIM_COUNTERMODE_UP; htim4.Init.Period = 999; htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim4);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);
}

static void MX_USART6_UART_Init(void) {
  __HAL_RCC_USART6_CLK_ENABLE();
  huart6.Instance = USART6; huart6.Init.BaudRate = 115200; huart6.Init.WordLength = UART_WORDLENGTH_8B; huart6.Init.StopBits = UART_STOPBITS_1; huart6.Init.Parity = UART_PARITY_NONE; huart6.Init.Mode = UART_MODE_TX_RX; huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE; huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart6);
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_GPIOC_CLK_ENABLE(); __HAL_RCC_GPIOB_CLK_ENABLE();

  // PA9 (LED Onboard / Relay Simulation) - Output
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA4 (PIR) & PA5 (DHT22)
  GPIO_InitStruct.Pin = GPIO_PIN_4 | DHT22_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA0 (MQ2) & PA1 (MQ135)
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint32_t ADC_Read_Channel(uint32_t channel) {
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();
  HAL_ADC_Start(&hadc1);
  if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
    uint32_t val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return val;
  }
  HAL_ADC_Stop(&hadc1);
  return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM4) ms_ticks++;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART6) {
    RB_Put(&wifi_rb, rx_data_temp);
    HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1);
  }
}

void Error_Handler(void) { __disable_irq(); while (1) {} }
