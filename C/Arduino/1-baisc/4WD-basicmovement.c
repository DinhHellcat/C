//left wheels
#define LW_f 2
#define LW_i 4
#define LW_EN 3
//right wheels
#define RW_f 6
#define RW_i 7
#define RW_EN 5

int defaultSpeed = 50;

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
}

void loop()
{
    turnRight();
    delay(500);

    turnLeft();
    delay(500);

    forward();
    delay(500);

    backward();
    delay(500);

    stop();
    delay(500);
}

void turnRight()
{
    setMotor(HIGH, LOW, LOW, HIGH, defaultSpeed, defaultSpeed);
}

void turnLeft()
{
    setMotor(LOW, HIGH, HIGH, LOW, defaultSpeed, defaultSpeed);
}

void forward()
{
    setMotor(HIGH, LOW, HIGH, LOW, defaultSpeed, defaultSpeed);
}

void backward()
{
    setMotor(LOW, HIGH, LOW, HIGH, defaultSpeed, defaultSpeed);
}

void stop()
{
    setMotor(LOW, LOW, LOW, LOW, 0, 0);
}

void setMotor(int lw_f, int lw_i, int rw_f, int rw_i, int lw_EN, int rw_EN)
{
    //left wheels
    digitalWrite(LW_f, lw_f);
    digitalWrite(LW_i, lw_i);
    analogWrite(LW_EN, lw_EN); //speed
    //right wheels
    digitalWrite(RW_f, rw_f);
    digitalWrite(RW_i, rw_i);
    analogWrite(RW_EN, rw_EN); //speed
}
