#include "ps2motors.h"

void setup()
{
  Serial.begin(115200);

  // init Motors
  Wire.begin(); // SDA, SCL
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(1600);
  Wire.setClock(400000);

  setMotor(0, 0);
  
  // setup PS2 controller
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }
  if (err == 0) Serial.println("Ket noi tay cam thanh cong");
  
  // serial print
  Serial.println("Done setup!");
}

void loop()
{
  ps2x.read_gamepad(0, 0);
  PS2control();
  delay(50);
}