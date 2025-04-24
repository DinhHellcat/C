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

int maxSpeed = 255;

void setup()
{
    //left wheels
    pinMode(LW_f, OUTPUT);
    pinMode(LW_i, OUTPUT);
    pinMode(LW_EN, OUTPUT);
    //right wheels
    pinMode(RW_f, OUTPUT);
    pinMode(RW_i, OUTPUT);
    pinMode(RW_EN, OUTPUT);
    //line sensor
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
}

int temp = 0;

void loop() {
    int sensors[5] = {
        digitalRead(D1),
        digitalRead(D2),
        digitalRead(D3),
        digitalRead(D4),
        digitalRead(D5)
    };

    int sensorValue = (sensors[0] << 4) | (sensors[1] << 3) | (sensors[2] << 2) | (sensors[3] << 1) | sensors[4];

    switch (sensorValue) {
        case 0b11111: //out line
            stop();
            if(temp == -1)
            {
                shapeLeft();
                while(digitalRead(D1) == 1) delay(10);
            }
            else
            {
                shapeRight();
                while(digitalRead(D5) == 1) delay(10);
            }
            break;
        //hard left
        case 0b00101:
            forward();
            delay(200);
            hardLeft();
            temp = -1;
            break;
        case 0b00001:
        case 0b01001:
            while(digitalRead(D3) == 0)
                forward();
            hardLeft();
            delay(50);
            temp = -1;
            break;
        //hard right
        case 0b10100:
            forward();
            delay(200);
            hardRight();
            temp = 11;
            break;
        case 0b10000:
        case 0b10010:
            while(digitalRead(D3) == 0)
                forward();
            hardRight();
            delay(50);
            temp = 1;
            break;
        //shape left
        case 0b01111:
        case 0b00111:
        case 0b00011:
            shapeLeft();
            temp = -1;
            break;

        //shape right
        case 0b11110:
        case 0b11100:
        case 0b11000:
            shapeRight();
            temp = 1;
            break;
        //normal left
        case 0b10111:
        // case 0b10011:
            turnLeft();
            break;
        //normal right
        case 0b11101:
        // case 0b11001:
            turnRight();
            break;

        // case 0b00000:
        //     // if(temp == -1)
        //     // {
        //     //     shapeLeft();
        //     //     while(digitalRead(D5) == 0) delay(10);
        //     //     break;
        //     // }
        //     // else
        //     // {
        //     //     shapeRight();
        //     //     while(digitalRead(D1) == 0) delay(10);
        //     //     break;
        //     // }
        //     stop();
        //     break;

        default:
            forward();
            break;
    }
}

void hardRight()
{
    setMotor(HIGH, LOW, LOW, LOW, maxSpeed, 0);
}

void hardLeft()
{
    setMotor(LOW, LOW, HIGH, LOW, 0, maxSpeed);
}

void shapeRight()
{
    setMotor(HIGH, LOW, LOW, HIGH, maxSpeed, maxSpeed);
}

void shapeLeft()
{
    setMotor(LOW, HIGH, HIGH, LOW, maxSpeed, maxSpeed);
}

void turnRight()
{
    setMotor(HIGH, LOW, HIGH, LOW, maxSpeed, maxSpeed-100);
}

void turnLeft()
{
    setMotor(HIGH, LOW, HIGH, LOW, maxSpeed-100, maxSpeed);
}

void forward()
{
    setMotor(HIGH, LOW, HIGH, LOW, maxSpeed, maxSpeed);
}

void backward()
{
    setMotor(LOW, HIGH, LOW, HIGH, maxSpeed, maxSpeed);
}

void stop()
{
    setMotor(LOW, LOW, LOW, LOW, 0, 0);
}

void setMotor(uint8_t lw_f, uint8_t lw_i, uint8_t rw_f, uint8_t rw_i, uint8_t lw_speed, uint8_t rw_speed) {
    // Left wheel
    digitalWrite(LW_f, lw_f);  // D6
    digitalWrite(LW_i, lw_i);  // D7
    analogWrite(LW_EN, lw_speed);  // D5

    // Right wheel
    digitalWrite(RW_f, rw_f);  // D4
    digitalWrite(RW_i, rw_i);  // D2
    analogWrite(RW_EN, rw_speed);  // D3
}
