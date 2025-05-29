#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PWM channels of pca9685 0-16
#define LF 8
#define LI 9
#define RF 11
#define RI 10

#define max_speed 4000
#define half_speed 2000
int speed = half_speed;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setMotor(int ls, int rs) {
  if (ls >= 0) {
    pwm.setPin(LF, ls);
    pwm.setPin(LI, 0);
  } else {
    pwm.setPin(LF, 0);
    pwm.setPin(LI, abs(ls));
  }

  if (rs >= 0) {
    pwm.setPin(RF, rs);
    pwm.setPin(RI, 0);
  } else {
    pwm.setPin(RF, 0);
    pwm.setPin(RI, abs(rs));
  }
}

void forward() {
  setMotor(speed, speed);
}
void forward_left() {
  setMotor(speed/2, speed);
}
void forward_right() {
  setMotor(speed, speed/2);
}
void left() {
  setMotor(-speed, speed);
}
void stop() {
  setMotor(0, 0);
}
void right() {
  setMotor(speed, -speed);
}
void backward_left() {
  setMotor(speed/2, -speed);
}
void backward() {
  setMotor(-speed, -speed);
}
void backward_right() {
  setMotor(-speed, speed/2);
}

// void setPWMMotors(int c1, int c2, int c3, int c4)
// {
//   char dbg_str[30];
//   sprintf(dbg_str,"C1: %d\tC2: %d\tC3: %d\tC4: %d",c1,c2,c3,c4);
//   Serial.println(dbg_str);

//   pwm.setPin(LF, c1);
//   pwm.setPin(LI, c2);
//   pwm.setPin(RF, c3);
//   pwm.setPin(RI, c4);
// }