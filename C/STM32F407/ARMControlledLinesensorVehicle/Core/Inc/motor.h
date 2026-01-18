/* file: motor.h */
#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h" // Để lấy định nghĩa các chân GPIO

/* --- Cấu hình tốc độ --- */
#define maxSpeed 40000
#define halfSpeed 27000
#define minSpeed 0

/* --- Nguyên mẫu hàm --- */
void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint16_t lw_speed, uint16_t rw_speed);

void forward(void);
void backward(void);
void stop(void);

void turnRight(void);
void turnLeft(void);

void hardRight(void);
void hardLeft(void);

void shapeRight(void);
void shapeLeft(void);

void backRight(void);
void backLeft(void);

void hardBackRight(void);
void hardBackLeft(void);

#endif
