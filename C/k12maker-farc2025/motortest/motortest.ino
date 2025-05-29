#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define LF 8
#define LI 9
#define RF 11
#define RI 10

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
  setMotor(2000, 2000);
}
void backward() {
  setMotor(-2000, -2000);
}
void turnLeft() {
  setMotor(-2000, 2000);
}
void turnRight() {
  setMotor(2000, -2000);
}
void stop() {
  setMotor(0, 0);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Motor test firmware for MakerBot 2024");

  pwm.begin();
  pwm.setPWMFreq(50);
}

void loop() {
  forward();
  delay(2000);

  stop();
  delay(500);

  backward();
  delay(2000);

  stop();
  delay(500);

  turnLeft();
  delay(2000);

  stop();
  delay(500);

  turnRight();
  delay(2000);

  stop();
  delay(500);
}