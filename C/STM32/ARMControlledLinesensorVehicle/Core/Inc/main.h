#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void Error_Handler(void);

////////////
#define lineSensorPort GPIOE
#define D1 GPIO_PIN_2
#define D2 GPIO_PIN_3
#define D3 GPIO_PIN_4
#define D4 GPIO_PIN_5
#define D5 GPIO_PIN_6

#define motorPort GPIOA
#define LW_f GPIO_PIN_12
#define LW_i GPIO_PIN_10
#define LW_EN GPIO_PIN_8
#define RW_f GPIO_PIN_15
#define RW_i GPIO_PIN_11
#define RW_EN GPIO_PIN_9

#define maxSpeed 45000
#define halfSpeed 30000
#define minSpeed 0

#define LEDPort GPIOA
#define LED1 GPIO_PIN_6
#define LED2 GPIO_PIN_7

#define Delay 50
////////////

#ifdef __cplusplus
}
#endif

#endif
