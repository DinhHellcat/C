/* file: main.h */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

/* --- Định nghĩa phần cứng (Hardware Pins) --- */
#define lineSensorPort GPIOE
#define D1 GPIO_PIN_2 // Trái ngoài cùng
#define D2 GPIO_PIN_3
#define D3 GPIO_PIN_4 // Giữa
#define D4 GPIO_PIN_5
#define D5 GPIO_PIN_6 // Phải ngoài cùng

#define motorPort GPIOA
#define LW_f GPIO_PIN_12
#define LW_i GPIO_PIN_10
#define LW_EN GPIO_PIN_8
#define RW_f GPIO_PIN_15
#define RW_i GPIO_PIN_11
#define RW_EN GPIO_PIN_9

#define LEDPort GPIOA
#define LED1 GPIO_PIN_6
#define LED2 GPIO_PIN_7

/* --- Định nghĩa Logic Cảm biến (Logic: 0=Line, 1=Nền) --- */
// 11111: Không thấy line
#define SENSOR_LOST         0b11111

// Các trường hợp đi thẳng (Line ở giữa hoặc lệch rất nhẹ)
#define SENSOR_CENTER       0b11011
#define SENSOR_SLIGHT_L     0b10011
#define SENSOR_SLIGHT_R     0b11001

// Các trường hợp lệch nhẹ -> Cua thường
#define SENSOR_TURN_L       0b10111
#define SENSOR_TURN_R       0b11101

// Các trường hợp góc vuông hoặc lệch nhiều -> Cua gấp (Hard Turn)
#define SENSOR_HARD_L       0b00111
#define SENSOR_HARD_R       0b11100

// Các trường hợp cua gắt (Shape Turn - Xoay tại chỗ)
#define SENSOR_SPIN_L_1     0b00011
#define SENSOR_SPIN_L_2     0b01111
#define SENSOR_SPIN_R_1     0b11000
#define SENSOR_SPIN_R_2     0b11110

#define Delay 50

#ifdef __cplusplus
}
#endif

#endif
