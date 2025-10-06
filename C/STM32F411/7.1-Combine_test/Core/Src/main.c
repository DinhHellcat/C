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
#include "dhtxx.h"
#include "dwt.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GAS_ALARM_THRESHOLD 1000 // Ngưỡng báo động cho cảm biến MQ-2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
// --- Handles cho các thư viện và ngoại vi ---
I2C_LCD_HandleTypeDef lcd;
DHTxx_Drv_t dht22;

// --- Biến lưu giá trị các cảm biến ---
float temperature, humidity;
uint32_t pir_state;
uint32_t adc_dma_buffer[2]; // Mảng chứa 2 giá trị ADC từ DMA

// --- Các chuỗi ký tự để hiển thị lên LCD ---
char line1_str[17];
char line2_str[17];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
static void Manual_DMA_Link_Init(void); // Hàm cấu hình DMA bằng tay
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* --- KHỐI KHỞI TẠO CỦA STM32 --- */
  HAL_Init();
  SystemClock_Config();

  /* --- KHỞI TẠO CÁC NGOẠI VI ĐÃ CẤU HÌNH TRONG CUBEMX --- */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  /* --- KHỞI TẠO CÁC THƯ VIỆN VÀ BẮT ĐẦU CÁC TIẾN TRÌNH --- */
  DwtInit();

  // Khởi tạo LCD
  lcd.hi2c = &hi2c1;
  lcd.address = 0x4E;
  lcd_init(&lcd);

  // Khởi tạo cảm biến DHT22
  dht22.DataPort = DHT22_GPIO_Port;
  dht22.DataPin = DHT22_Pin;
  dht22.Type = DHT22;

  // Cấu hình và liên kết DMA bằng tay để đảm bảo hoạt động
  Manual_DMA_Link_Init();

  // Bắt đầu quá trình đọc ADC bằng DMA
  HAL_ADC_Start_DMA(&hadc1, adc_dma_buffer, 2);

  // Bắt đầu phát xung PWM cho Servo
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

  // Thiết lập trạng thái ban đầu cho các thiết bị
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500); // Đóng cửa
  lcd_puts(&lcd, "System ready!");
  HAL_Delay(2000);
  lcd_clear(&lcd);
  /* USER CODE END 2 */

  /* --- VÒNG LẶP CHÍNH CỦA CHƯƠNG TRÌNH --- */
  while (1)
  {
    /* --- 1. ĐỌC DỮ LIỆU TỪ TẤT CẢ CẢM BIẾN --- */
    DHT_GetData(&dht22);
    temperature = dht22.Temperature;
    humidity = dht22.Humidity;
    pir_state = HAL_GPIO_ReadPin(PIR_GPIO_Port, PIR_Pin);

    // Lấy giá trị MQ-2 và MQ-135 trực tiếp từ bộ đệm DMA đã được cập nhật ở nền
    uint32_t mq2_value = adc_dma_buffer[0];
    uint32_t mq135_value = adc_dma_buffer[1];

    /* --- 2. XỬ LÝ LOGIC VÀ ĐIỀU KHIỂN (STATE MACHINE) --- */
    // Ưu tiên cao nhất: Báo động khí gas
    if (mq2_value > GAS_ALARM_THRESHOLD)
    {
      HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
      sprintf(line1_str, "CANH BAO GAS!  ");
      sprintf(line2_str, "Muc do: %-5lu", mq2_value);
    }
    // Ưu tiên thứ hai: Phát hiện chuyển động
    else if (pir_state == GPIO_PIN_SET)
    {
      HAL_GPIO_WritePin(LED_ON_BOARD_GPIO_Port, LED_ON_BOARD_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1500); // Mở cửa
      sprintf(line1_str, "Phat hien       ");
      sprintf(line2_str, "chuyen dong...  ");
    }
    // Trạng thái bình thường
    else
    {
      HAL_GPIO_WritePin(LED_ON_BOARD_GPIO_Port, LED_ON_BOARD_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500); // Đóng cửa
      sprintf(line1_str, "NhietDo: %.1f C", temperature);
      sprintf(line2_str, "AQI Raw: %-5lu", mq135_value);
    }

    /* --- 3. CẬP NHẬT MÀN HÌNH LCD --- */
    lcd_clear(&lcd);
    lcd_gotoxy(&lcd, 0, 0);
    lcd_puts(&lcd, line1_str);
    lcd_gotoxy(&lcd, 0, 1);
    lcd_puts(&lcd, line2_str);

    HAL_Delay(1000);
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  // Cấu hình ADC chung
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE; // Bật yêu cầu DMA liên tục
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  // Cấu hình kênh 1 (Rank 1) cho MQ-2 (PA0)
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  // Cấu hình kênh 2 (Rank 2) cho MQ-135 (PA1)
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 99;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Bật Preload
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) // Chỉ dùng PWM_Init là đủ
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500; // Đặt giá trị ban đầu
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim2);
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_ON_BOARD_GPIO_Port, LED_ON_BOARD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUZZER_Pin */
  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PIR_Pin DHT22_Pin */
  GPIO_InitStruct.Pin = PIR_Pin|DHT22_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_ON_BOARD_Pin */
  GPIO_InitStruct.Pin = LED_ON_BOARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_ON_BOARD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SERVO_Pin */
  GPIO_InitStruct.Pin = SERVO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(SERVO_GPIO_Port, &GPIO_InitStruct);

  /* === BỔ SUNG CẤU HÌNH CHO 2 CHÂN ADC (PA0, PA1) === */
  GPIO_InitStruct.Pin = MQ_2_Pin|MQ_135_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; // Chế độ Analog
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* ================================================= */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Hàm cấu hình và liên kết DMA với ADC một cách thủ công.
  * @note   Hàm này được tạo ra để khắc phục lỗi generate code của CubeMX,
  * đảm bảo ADC và DMA luôn được liên kết đúng cách.
  * @retval None
  */
static void Manual_DMA_Link_Init(void)
{
  // 1. Cấu hình chi tiết cho luồng DMA mà ADC1 sẽ sử dụng
  hdma_adc1.Instance = DMA2_Stream0;
  hdma_adc1.Init.Channel = DMA_CHANNEL_0;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
  {
    Error_Handler();
  }

  // 2. "Giới thiệu" ADC với DMA. Đây là bước quan trọng nhất!
  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
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
