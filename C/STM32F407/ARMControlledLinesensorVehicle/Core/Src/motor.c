/* file: motor.c */
#include "motor.h"

/* Khai báo extern để sử dụng biến htim1 đã định nghĩa bên main.c */
extern TIM_HandleTypeDef htim1;

/* --- Hàm cốt lõi --- */
void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint16_t lw_speed, uint16_t rw_speed)
{
    // Bánh trái
    HAL_GPIO_WritePin(motorPort, LW_f, lw_f);
    HAL_GPIO_WritePin(motorPort, LW_i, lw_i);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, lw_speed);

    // Bánh phải
    HAL_GPIO_WritePin(motorPort, RW_f, rw_f);
    HAL_GPIO_WritePin(motorPort, RW_i, rw_i);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rw_speed);
}

/* --- Các hướng di chuyển cơ bản --- */
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

/* --- Các hướng rẽ --- */
void shapeRight()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, maxSpeed, maxSpeed);
}

void shapeLeft()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, maxSpeed, maxSpeed);
}

void turnRight()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, maxSpeed, halfSpeed);
}

void turnLeft()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, halfSpeed, maxSpeed);
}

void hardRight()
{
    setMotor(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, maxSpeed, 0);
}

void hardLeft()
{
    setMotor(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, 0, maxSpeed);
}

/* --- Các hướng lùi rẽ (Dành cho Manual Mode) --- */
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
