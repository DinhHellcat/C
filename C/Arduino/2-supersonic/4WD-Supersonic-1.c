#include <avr/io.h>
#include <util/delay.h>

#define TRIG_PIN 8
#define ECHO_PIN 9

//left wheels
#define LW_f 4
#define LW_i 2
#define LW_EN 3
//right wheels
#define RW_f 6
#define RW_i 7
#define RW_EN 5

int defaultSpeed = 200;

void setup()
{
    //left wheels
    DDRD |= (1 << PD2) | (1 << PD4);
    pinMode(LW_EN, OUTPUT);
    //right wheels
    DDRD |= (1 << PD6) | (1 << PD7);
    pinMode(RW_EN, OUTPUT);
    //ultrasonic
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
  uint16_t distance = readDistanceCM();
  if (distance < 20) {
    stop();
  } else {
    forward();
  }
  delay(100);
}

void forward()
{
    setMotor(HIGH, LOW, HIGH, LOW, defaultSpeed, defaultSpeed);
}

void stop()
{
    setMotor(LOW, LOW, LOW, LOW, 0, 0);
}

void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint8_t lw_speed, uint8_t rw_speed) {
  // Left wheel
  if (lw_f) PORTD |= (1 << PD4); else PORTD &= ~(1 << PD4);  // D2
  if (lw_i) PORTD |= (1 << PD2); else PORTD &= ~(1 << PD2);  // D4
  analogWrite(LW_EN, lw_speed);  // D3

  // Right wheel
  if (rw_f) PORTD |= (1 << PD6); else PORTD &= ~(1 << PD6);  // D6
  if (rw_i) PORTD |= (1 << PD7); else PORTD &= ~(1 << PD7);  // D7
  analogWrite(RW_EN, rw_speed);  // D5
}

uint16_t readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Đợi ECHO lên cao
  while (digitalRead(ECHO_PIN) == LOW);
  unsigned long startTime = micros();
  while (digitalRead(ECHO_PIN) == HIGH);
  unsigned long travelTime = micros() - startTime;

  uint16_t distance = travelTime * 0.034 / 2;
  return distance;
}
