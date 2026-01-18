/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Hybrid Mode: Blocking Connect -> Non-blocking Data
  * @version        : 2.0
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
#include "ring_buffer.h"

/* Private defines -----------------------------------------------------------*/
// CẤU HÌNH MẠNG (Hãy chắc chắn thông tin này khớp với Hotspot hiện tại)
#define WIFI_SSID       "Herukyatto"  // Tên Hotspot (phân biệt hoa thường)
#define WIFI_PASSWORD   "123456789"        // Pass Hotspot
#define SERVER_IP       "192.168.137.120"     // IP Laptop trong mạng Hotspot
#define SERVER_PORT     5000

#define SEND_INTERVAL   5000

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart6;

I2C_LCD_HandleTypeDef lcd;
DHTxx_Drv_t dht22;

volatile uint32_t ms_ticks = 0;
RingBuffer wifi_rb;
uint8_t rx_data_temp;

// Buffer dùng cho hàm blocking cũ
char response_buffer[512];
uint16_t response_index = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART6_UART_Init(void);
void Error_Handler(void);

// Hàm Blocking cũ (đã được phục hồi)
int send_at_command(char* command, char* expected_response, uint32_t timeout);
void WiFi_Connect_Hybrid(void);

/**
  * @brief  The application entry point.
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_USART6_UART_Init();
  DwtInit();

  // 1. Khởi tạo LCD & DHT
  lcd.hi2c = &hi2c1;
  lcd.address = 0x4E;
  lcd_init(&lcd);
  lcd_clear(&lcd);
  lcd_puts(&lcd, "System Booting");

  dht22.DataPort = DHT22_GPIO_Port;
  dht22.DataPin = DHT22_Pin;
  dht22.Type = DHT22;

  // 2. KẾT NỐI WIFI (Dùng cách cũ Blocking)
  // Lưu ý: Lúc này chưa bật ngắt UART để tránh xung đột với hàm blocking
  WiFi_Connect_Hybrid();

  // 3. CHUYỂN SANG CHẾ ĐỘ 2 CHIỀU (Non-blocking)
  // Sau khi đã nối thông server và vào mode Transparent, ta bật RingBuffer
  RB_Init(&wifi_rb);
  HAL_UART_Receive_IT(&huart6, &rx_data_temp, 1); // Bắt đầu hứng dữ liệu từ Server
  HAL_TIM_Base_Start_IT(&htim4);

  lcd_clear(&lcd);
  lcd_puts(&lcd, "System Ready!");
  HAL_Delay(1000);

  uint32_t last_send_time = 0;

  /* --- 4. VÒNG LẶP CHÍNH --- */
  while (1)
  {
      // --- A. Gửi dữ liệu lên (Uplink) ---
      if (ms_ticks - last_send_time >= SEND_INTERVAL)
      {
          last_send_time = ms_ticks;
          DHT_GetData(&dht22);

          // Format JSON gọn nhẹ
          char json_msg[64];
          sprintf(json_msg, "{\"t\":%.1f,\"h\":%.1f}\n", dht22.Temperature, dht22.Humidity);

          // Gửi đi (Transparent mode nên cứ bắn là đi)
          HAL_UART_Transmit(&huart6, (uint8_t*)json_msg, strlen(json_msg), 100);

          lcd_gotoxy(&lcd, 0, 0);
          lcd_puts(&lcd, "Sent Data...    ");
      }

      // --- B. Nhận lệnh xuống (Downlink - Realtime) ---
      if (!RB_IsEmpty(&wifi_rb))
      {
          uint8_t data;
          static int col = 0;
          if (col == 0) {
              lcd_gotoxy(&lcd, 0, 1);
              lcd_puts(&lcd, "Rx:             "); // Xóa dòng 2
              lcd_gotoxy(&lcd, 0, 1);
              lcd_puts(&lcd, "Rx: ");
              col = 4;
          }

          // Đọc hết dữ liệu trong buffer
          while (RB_Get(&wifi_rb, &data))
          {
              // Hiển thị lên LCD để test 2 chiều
              if (data >= 32 && data <= 126)
              {
                  char s[2] = {data, 0};
                  lcd_puts(&lcd, s);
                  col++;
                  if (col > 15) col = 0;
              }
              // Sau này ta sẽ thêm parser JSON ở đây:
              // if (data == '}') { ProcessJSON(...); }
          }
      }
  }
}

/* ============================================================ */
/* CÁC HÀM HỖ TRỢ (Hybrid)                             */
/* ============================================================ */

// Hàm gửi AT Blocking (Cũ của bạn - Đã chỉnh sửa nhẹ để chạy độc lập)
int send_at_command(char* command, char* expected_response, uint32_t timeout)
{
    // Xóa buffer cũ
    memset(response_buffer, 0, sizeof(response_buffer));
    response_index = 0;

    // Gửi lệnh
    HAL_UART_Transmit(&huart6, (uint8_t*)command, strlen(command), 100);

    // Chờ phản hồi (Polling thủ công thay vì ngắt để tránh phức tạp giai đoạn này)
    uint32_t start_time = HAL_GetTick();
    while (HAL_GetTick() - start_time < timeout)
    {
        uint8_t byte;
        // Kiểm tra xem có dữ liệu trong thanh ghi DR không
        if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE))
        {
            HAL_UART_Receive(&huart6, &byte, 1, 10); // Đọc 1 byte
            if (response_index < sizeof(response_buffer) - 1)
            {
                response_buffer[response_index++] = byte;
                response_buffer[response_index] = 0; // Null terminate

                // Kiểm tra xem chuỗi kỳ vọng có xuất hiện không
                if (strstr(response_buffer, expected_response) != NULL)
                {
                    return 1; // Thành công
                }
            }
        }
    }
    return 0; // Timeout
}

void WiFi_Connect_Hybrid(void)
{
    lcd_clear(&lcd);
    lcd_puts(&lcd, "Checking AT...");

    // 1. Kiểm tra AT
    if (send_at_command("AT\r\n", "OK", 1000)) {
        lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "AT OK");
    } else {
        lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "AT FAIL");
        HAL_Delay(1000); // Cứ thử tiếp xem sao
    }
    HAL_Delay(1000);

    lcd_clear(&lcd); lcd_puts(&lcd, "Resetting...");

    // 2. Reset & Cấu hình Mode (Code cũ của bạn)
    send_at_command("AT+RST\r\n", "ready", 5000); // Đợi chữ ready
    HAL_Delay(2000);
    send_at_command("ATE0\r\n", "OK", 1000);      // Tắt echo

    // Dùng lệnh WMODE gốc của module combo
    if(send_at_command("AT+WMODE=1,1\r\n", "OK", 3000)) {
        lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Station Mode OK");
    }
    HAL_Delay(1000);

    // 3. Kết nối Wifi (QUAN TRỌNG)
    lcd_clear(&lcd); lcd_puts(&lcd, "Connecting WiFi...");
    char connect_cmd[128];
    // Dùng lệnh WJAP gốc của module combo [cite: 30]
    sprintf(connect_cmd, "AT+WJAP=%s,%s\r\n", WIFI_SSID, WIFI_PASSWORD);

    // Chờ +EVENT:WIFI_GOT_IP như code cũ bạn cung cấp
    if (send_at_command(connect_cmd, "+EVENT:WIFI_GOT_IP", 20000))
    {
        lcd_clear(&lcd); lcd_puts(&lcd, "WiFi Connected!");
    }
    else
    {
        lcd_clear(&lcd); lcd_puts(&lcd, "WiFi Failed!");
        // Nếu fail, ta vẫn thử kết nối Server xem có phép màu không (đôi khi buffer lỡ nhịp)
    }
    HAL_Delay(2000);

    // 4. Kết nối TCP Server (Bước đệm để lên 2 chiều)
    lcd_clear(&lcd); lcd_puts(&lcd, "Linking Server..");
    char server_cmd[128];
    // Dùng lệnh SOCKET chuẩn của module combo (Thay vì CIPSTART) [cite: 50]
    // Type 4 = TCP Client
    sprintf(server_cmd, "AT+SOCKET=4,%s,%d\r\n", SERVER_IP, SERVER_PORT);

    if (send_at_command(server_cmd, "connect success", 10000)) {
        lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Server OK!");
    } else {
        lcd_gotoxy(&lcd, 0, 1); lcd_puts(&lcd, "Server Fail!");
        // Có thể server chưa bật, nhưng cứ đi tiếp để test luồng
    }
    HAL_Delay(2000);

    // 5. Vào chế độ Transparent (AT+SOCKETTT) [cite: 52]
    // Để sau hàm này, mọi thứ ta gửi bằng HAL_UART_Transmit sẽ bay thẳng tới server
    send_at_command("AT+SOCKETTT\r\n", ">", 3000);

    lcd_clear(&lcd);
}

/* USER CODE BEGIN 4 */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

/* GIỮ NGUYÊN CÁC HÀM INIT KHÁC (SystemClock, MX_...) */
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) Error_Handler();
}
static void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1; hi2c1.Init.ClockSpeed = 100000; hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; hi2c1.Init.OwnAddress1 = 0; hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; hi2c1.Init.OwnAddress2 = 0; hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) Error_Handler();
}
static void MX_TIM4_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0}; TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim4.Instance = TIM4; htim4.Init.Prescaler = 99; htim4.Init.CounterMode = TIM_COUNTERMODE_UP; htim4.Init.Period = 999; htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK) Error_Handler();
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK) Error_Handler();
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK) Error_Handler();
}
static void MX_USART6_UART_Init(void) {
  huart6.Instance = USART6; huart6.Init.BaudRate = 115200; huart6.Init.WordLength = UART_WORDLENGTH_8B; huart6.Init.StopBits = UART_STOPBITS_1; huart6.Init.Parity = UART_PARITY_NONE; huart6.Init.Mode = UART_MODE_TX_RX; huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE; huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK) Error_Handler();
}
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOH_CLK_ENABLE(); __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_GPIOC_CLK_ENABLE(); __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = DHT22_Pin; GPIO_InitStruct.Mode = GPIO_MODE_INPUT; GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
/* USER CODE END 4 */
