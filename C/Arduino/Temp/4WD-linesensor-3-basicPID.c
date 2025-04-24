//left wheels
#define LW_f 6
#define LW_i 7
#define LW_EN 5
//right wheels
#define RW_f 4
#define RW_i 2
#define RW_EN 3
//line sensor
#define D1 A0
#define D2 A1
#define D3 A2
#define D4 A3
#define D5 A4

int defaultSpeed = 200;

void setup()
{
    //left wheels
    DDRD |= (1 << PD2) | (1 << PD4);
    pinMode(LW_EN, OUTPUT);
    //right wheels
    DDRD |= (1 << PD6) | (1 << PD7);
    pinMode(RW_EN, OUTPUT);
    //line sensor
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
}

int weights[5] = {-2, -1, 0, 1, 2}; // trái âm, phải dương

void loop() {
    int sensors[5] = {
        digitalRead(D1),
        digitalRead(D2),
        digitalRead(D3),
        digitalRead(D4),
        digitalRead(D5)
    };

    int error = 0;
    int activeCount = 0;

    for (int i = 0; i < 5; i++) {
        if (sensors[i] == 1) {
            error += weights[i];
            activeCount++;
        }
    }

    if (activeCount == 0) {
      stop();
      return;
    }
    else if (activeCount == 5)
    {
      backward();
      return;
    }

    int baseSpeed = defaultSpeed;
    int correction = error * 40;

    int leftSpeed = constrain(baseSpeed - correction, 0, 255);
    int rightSpeed = constrain(baseSpeed + correction, 0, 255);

    setMotor(HIGH, LOW, HIGH, LOW, leftSpeed, rightSpeed);
}

void backward()
{
    setMotor(LOW, HIGH, LOW, HIGH, defaultSpeed, defaultSpeed);
}

void stop()
{
    setMotor(LOW, LOW, LOW, LOW, 0, 0);
}

void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint8_t lw_speed, uint8_t rw_speed) {
  // Left wheel
  if (lw_f) PORTD |= (1 << PD6); else PORTD &= ~(1 << PD6);  // D2
  if (lw_i) PORTD |= (1 << PD7); else PORTD &= ~(1 << PD7);  // D4
  analogWrite(LW_EN, lw_speed);  // D3

  // Right wheel
  if (rw_f) PORTD |= (1 << PD4); else PORTD &= ~(1 << PD4);  // D6
  if (rw_i) PORTD |= (1 << PD2); else PORTD &= ~(1 << PD2);  // D7
  analogWrite(RW_EN, rw_speed);  // D5
}
