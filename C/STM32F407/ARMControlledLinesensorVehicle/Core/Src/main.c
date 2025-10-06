#include "main.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart4;
uint8_t rx_data;

volatile uint8_t mode = 1; // 0: MANUAL, 1: AUTONOMOUS
volatile int temp = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_UART4_Init(void);

// Các hàm điều khiển motor
void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint16_t lw_speed, uint16_t rw_speed);
void hardRight(void);
void hardLeft(void);
void shapeRight(void);
void shapeLeft(void);
void turnRight(void);
void turnLeft(void);
void forward(void);
void backward(void);
void stop(void);
void backRight(void);
void backLeft(void);
void hardBackLeft(void);
void hardBackRight(void);

/* Main application ----------------------------------------------------------*/
int main(void)
{
  /* MCU Configuration */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_UART4_Init();

  /* Peripheral and Interrupt Initialization */
  HAL_UART_Receive_IT(&huart4, &rx_data, 1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

  // Báo hiệu khởi động
  HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_SET);

  // Bắt đầu đi thẳng
  forward();

  /* Infinite loop */
  while (1)
  {
	  if (mode == 1) // AUTONOMOUS mode
	  {
          HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_RESET);

		  int sensors[5];
		  sensors[0] = HAL_GPIO_ReadPin(lineSensorPort, D1);
		  sensors[1] = HAL_GPIO_ReadPin(lineSensorPort, D2);
		  sensors[2] = HAL_GPIO_ReadPin(lineSensorPort, D3);
		  sensors[3] = HAL_GPIO_ReadPin(lineSensorPort, D4);
		  sensors[4] = HAL_GPIO_ReadPin(lineSensorPort, D5);

		  char sensor = (sensors[0] << 4) | (sensors[1] << 3) | (sensors[2] << 2) | (sensors[3] << 1) | sensors[4];
		  sensor = sensor & 0b00011111;

		  switch (sensor)
		  {
		  	  case 0b11111:
	              stop();
	              HAL_Delay(Delay);
	              backward();
	              HAL_Delay(Delay);
		  		  if(temp == -1)
		  		  {
		  			  shapeLeft();
					  while (HAL_GPIO_ReadPin(lineSensorPort, D1) == GPIO_PIN_SET && mode == 1) HAL_Delay(10);
		  		  }
		  		  else if (temp == 1)
		  		  {
		  			  shapeRight();
		  			  while (HAL_GPIO_ReadPin(lineSensorPort, D5) == GPIO_PIN_SET && mode == 1) HAL_Delay(10);
		  		  }
		  		  stop();
	              HAL_Delay(Delay);
	              forward();
		  		  break;
		  	  case 0b11011:
		  	  case 0b10011:
		  	  case 0b11001:
		  		  forward();
		  		  break;
		  	  case 0b10111:
		  		  turnLeft();
		  		  break;
		  	  case 0b11101:
		  		  turnRight();
		  		  break;
		  	  case 0b00111:
		  		  hardLeft();
		  		  temp = -1;
		  		  break;
		  	  case 0b11100:
		  		  hardRight();
		  		  temp = 1;
		  		  break;
		  	  case 0b00011:
		  	  case 0b01111:
		  		  shapeLeft();
		  		  HAL_Delay(Delay);
		  		  temp = -1;
		  		  break;
		  	  case 0b11000:
		  	  case 0b11110:
		  		  shapeRight();
		  		  HAL_Delay(Delay);
		  		  temp = 1;
		  		  break;
			  default:
				  forward();
				  break;
		  }
	  }
	  else // MANUAL mode
	  {
          HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_SET);
	  }
  }
}

/******************************************************************************/
/* Callback and System Functions                         */
/******************************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart4)
{
    if (huart4->Instance == UART4)
    {
        switch (rx_data)
        {
            case 'S':
            case 'L':
            case 'R': // Resume autonomous mode
                mode = 1;
		  		stop(); // Stop motors before resuming line-following
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_SET);
                break;
            case 'F': // Forward
                mode = 0;
                forward();
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_RESET);
                break;
            case 'B': // Backward
                mode = 0;
                backward();
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_RESET);
                break;
            case 'G': // Forward left
                mode = 0;
                hardLeft();
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_SET);
                break;
            case 'H': // Forward right
                mode = 0;
                hardRight();
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_SET);
                break;
            case 'I': // Backward left
                mode = 0;
                hardBackLeft();
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_RESET);
                break;
            case 'J': // Backward right
                mode = 0;
                hardBackRight();
                HAL_GPIO_WritePin(LEDPort, LED1, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LEDPort, LED2, GPIO_PIN_RESET);
                break;
            default:
                break;
        }
    }
    // Re-enable UART interrupt
    HAL_UART_Receive_IT(&huart4, &rx_data, 1);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) Error_Handler();
}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim1);

}

static void MX_UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

/******************************************************************************/
/* Motor Driving Functions                               */
/******************************************************************************/

void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint16_t lw_speed, uint16_t rw_speed)
{
    // Left wheel
    HAL_GPIO_WritePin(motorPort, LW_f, lw_f);
    HAL_GPIO_WritePin(motorPort, LW_i, lw_i);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, lw_speed);

    // Right wheel
    HAL_GPIO_WritePin(motorPort, RW_f, rw_f);
    HAL_GPIO_WritePin(motorPort, RW_i, rw_i);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rw_speed);
}

void shapeRight()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, maxSpeed, maxSpeed);
}

void shapeLeft()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, maxSpeed, maxSpeed);
}

void forward()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, maxSpeed, maxSpeed);
}

void backward()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, maxSpeed, maxSpeed);
}

void stop()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, 0, 0);
}

void turnRight()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, maxSpeed, halfSpeed);
}

void turnLeft()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, halfSpeed, maxSpeed);
}

void backRight()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, maxSpeed, halfSpeed);
}

void backLeft()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, halfSpeed, maxSpeed);
}

void hardBackRight()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, maxSpeed, 0);
}

void hardBackLeft()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, 0, maxSpeed);
}

void hardRight()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, maxSpeed, 0);
}

void hardLeft()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, 0, maxSpeed);
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif
