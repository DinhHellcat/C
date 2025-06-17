#include <PS2X_lib.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <stdio.h>

PS2X ps2x; // create PS2 Controller Class object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define PS2_DAT 12  // MISO
#define PS2_CMD 13  // MOSI
#define PS2_SEL 15  // SS
#define PS2_CLK 14  // SLK

#define LF 8  // Left Forward
#define LI 9  // Left Inverse
#define RF 11 // Right Forward
#define RI 10 // Right Inverse

#define max_speed 4000
#define half_speed 2000
int speed = half_speed;
bool driving_speed = false;

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

void PS2control() {
  // Cập nhật trạng thái tay cầm
  ps2x.read_gamepad();

  // Chuyển đổi tốc độ khi nhấn PSB_L2 hoặc PSB_R2
  if (ps2x.ButtonPressed(PSB_L2) || ps2x.ButtonPressed(PSB_R2)) {
    Serial.println("Changing speed");
    driving_speed = !driving_speed;
    speed = driving_speed ? max_speed : half_speed;
  }

  // Điều khiển động cơ
  if (ps2x.Button(PSB_L1) && ps2x.Button(PSB_R1)) {
    // Nếu cả hai nút được giữ, dừng xe
    setMotor(0, 0);
  }
  else if (ps2x.Button(PSB_L1)) {
    // Sử dụng cần trái (PSS_LX và PSS_LY)
    int nJoyX = ps2x.Analog(PSS_LX) - 128; // Chuyển về -128 đến 127
    int nJoyY = ps2x.Analog(PSS_LY) - 128; // Chuyển về -128 đến 127

    // Tính toán tốc độ động cơ trái và phải
    int nMotMixL = nJoyY - nJoyX; // Trộn Y (tốc độ) và X (hướng)
    int nMotMixR = nJoyY + nJoyX;

    // Ánh xạ giá trị từ -128 đến 128 sang 0 đến speed
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    if (nMotMixL > 0) {
      c1 = map(nMotMixL, 0, 128, 0, speed);
    } else if (nMotMixL < 0) {
      c2 = map(abs(nMotMixL), 0, 128, 0, speed);
    }
    if (nMotMixR > 0) {
      c3 = map(nMotMixR, 0, 128, 0, speed);
    } else if (nMotMixR < 0) {
      c4 = map(abs(nMotMixR), 0, 128, 0, speed);
    }

    // Điều khiển động cơ
    setMotor(c1 - c2, c3 - c4); // Tính toán tốc độ net cho động cơ trái và phải
  }
  else if (ps2x.Button(PSB_R1)) {
    // Sử dụng cần phải (PSS_RX và PSS_RY)
    int nJoyX = ps2x.Analog(PSS_RX) - 128; // Chuyển về -128 đến 127
    int nJoyY = ps2x.Analog(PSS_RY) - 128; // Chuyển về -128 đến 127

    // Tính toán tốc độ động cơ trái và phải
    int nMotMixL = nJoyY - nJoyX;
    int nMotMixR = nJoyY + nJoyX;

    // Ánh xạ giá trị từ -128 đến 128 sang 0 đến speed
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    if (nMotMixL > 0) {
      c1 = map(nMotMixL, 0, 128, 0, speed);
    } else if (nMotMixL < 0) {
      c2 = map(abs(nMotMixL), 0, 128, 0, speed);
    }
    if (nMotMixR > 0) {
      c3 = map(nMotMixR, 0, 128, 0, speed);
    } else if (nMotMixR < 0) {
      c4 = map(abs(nMotMixR), 0, 128, 0, speed);
    }

    // Điều khiển động cơ
    setMotor(c1 - c2, c3 - c4);
  }
  else {
    // Không nhấn nút, dừng xe
    setMotor(0, 0);
  }

  delay(50); // Delay để tránh quá tải
}