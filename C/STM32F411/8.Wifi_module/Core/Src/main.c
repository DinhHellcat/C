/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_lcd.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
// --- Biến cho giao tiếp UART với Module Wi-Fi ---
I2C_LCD_HandleTypeDef lcd;
uint8_t uart_rx_byte;
char response_buffer[200];
uint8_t response_index = 0;
volatile uint8_t data_received_flag = 0;

typedef enum {
    WIFI_STATE_CONNECTING,
    WIFI_STATE_GET_INFO,
    WIFI_STATE_CONNECTED
} WifiState_t;

WifiState_t wifi_state = WIFI_STATE_CONNECTING;

char line1_str[20];
char line2_str[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LCD_Init_Custom()
{
  lcd.hi2c = &hi2c1;
  lcd.address = 0x4E;
  lcd_init(&lcd);
}

int send_at_command(const char* command, const char* expected_response, uint32_t timeout)
{
    // Xóa bộ đệm cũ
    memset(response_buffer, 0, sizeof(response_buffer));
    response_index = 0;

    // Gửi lệnh
    HAL_UART_Transmit(&huart6, (uint8_t*)command, strlen(command), 1000);

    // Chờ cho hết thời gian timeout để thu thập toàn bộ phản hồi
    HAL_Delay(timeout);

    // Sau khi đã chờ, kiểm tra xem chuỗi phản hồi thu thập được có chứa nội dung mong muốn không
    if (strstr(response_buffer, expected_response) != NULL)
    {
        return 1; // Thành công
    }

    return 0; // Thất bại
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* --- KHỞI TẠO --- */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART6_UART_Init();

  /* USER CODE BEGIN 2 */
  // Khởi tạo LCD và bắt đầu lắng nghe UART
  LCD_Init_Custom();
  HAL_UART_Receive_IT(&huart6, &uart_rx_byte, 1);

//  // Reset module và tắt echo
//  lcd_puts(&lcd, "Set up");
//  send_at_command("AT+RST\r\n", "ready", 5000);
//  HAL_Delay(2000);
//  send_at_command("ATE0\r\n", "OK", 1000);
//  HAL_Delay(500);
//  send_at_command("AT+WMODE=1,1\r\n", "OK", 3000);
//  HAL_Delay(500);

  // Gửi lệnh kết nối Wi-Fi một lần duy nhất
  const char* connect_command = "AT+WJAP=\"Herukyatto\",\"1234567890\"\r\n";
  lcd_clear(&lcd);
  lcd_puts(&lcd, "Dang ket noi...");
  HAL_UART_Transmit(&huart6, (uint8_t*)connect_command, strlen(connect_command), 1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    // Đây là một máy trạng thái (state machine) đơn giản
	    switch (wifi_state)
	    {
	      case WIFI_STATE_CONNECTING:
	        // Trạng thái chờ sự kiện WIFI_GOT_IP
	        // Dựa trên quan sát thực tế của bạn, chuỗi này có dấu gạch dưới
	        if (strstr(response_buffer, "+EVENT:WIFI_GOT_IP") != NULL)
	        {
	          lcd_clear(&lcd);
	          lcd_puts(&lcd, "KET NOI THANH CONG!");
	          HAL_Delay(2000);
	          wifi_state = WIFI_STATE_GET_INFO; // Chuyển sang trạng thái lấy thông tin
	        }
	        break;

	      case WIFI_STATE_GET_INFO:
	        // Trạng thái gửi lệnh lấy thông tin
	        lcd_clear(&lcd);
	        lcd_puts(&lcd, "Lay thong tin...");
	        // Sử dụng AT+WJAP? để lấy thông tin
	        if (send_at_command("AT+WJAP?\r\n", "OK", 3000))
	        {
	            // --- XỬ LÝ CHUỖI ĐỂ LẤY IP VÀ MAC ---
	            char ip_addr[16] = "Not Found";
	            char mac_addr[18] = "Not Found";
	            char temp_buffer[200];
	            strcpy(temp_buffer, response_buffer);

	            char *token = strtok(temp_buffer, ",");
	            int token_count = 0;
	            while(token != NULL)
	            {
	                // Dựa trên định dạng, IP là token thứ 8, MAC là token thứ 6
	                if(token_count == 5) {
	                    strncpy(mac_addr, token, 17);
	                    mac_addr[17] = '\0';
	                } else if(token_count == 7) {
	                    strncpy(ip_addr, token, 15);
	                    ip_addr[15] = '\0';
	                }
	                token = strtok(NULL, ",");
	                token_count++;
	            }

	            // In kết quả đã xử lý
	            lcd_clear(&lcd);
	            sprintf(line1_str, "IP: %s", ip_addr);
	            sprintf(line2_str, "MAC:%s", mac_addr);
	            lcd_puts(&lcd, line1_str);
	            lcd_gotoxy(&lcd, 0, 1);
	            lcd_puts(&lcd, line2_str);

	            wifi_state = WIFI_STATE_CONNECTED;
	        }
	        else
	        {
	            lcd_clear(&lcd);
	            lcd_puts(&lcd, "Loi lay thong tin");
	            HAL_Delay(2000);
	        }
	        break;

	      case WIFI_STATE_CONNECTED:
	        // Đã kết nối và lấy thông tin xong, không làm gì cả
	        break;
	    }
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_ON_BOARD_GPIO_Port, LED_ON_BOARD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_ON_BOARD_Pin */
  GPIO_InitStruct.Pin = LED_ON_BOARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_ON_BOARD_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART6)
  {
    // Chỉ ghép ký tự vào bộ đệm, không đặt cờ báo nữa
    if (response_index < sizeof(response_buffer) - 1)
    {
      // Thay thế ký tự xuống dòng bằng dấu cách để dễ hiển thị trên 1 dòng
      if(uart_rx_byte == '\r' || uart_rx_byte == '\n')
      {
          response_buffer[response_index++] = ' ';
      }
      else
      {
          response_buffer[response_index++] = uart_rx_byte;
      }
    }

    // Luôn lắng nghe byte tiếp theo
    HAL_UART_Receive_IT(&huart6, &uart_rx_byte, 1);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
