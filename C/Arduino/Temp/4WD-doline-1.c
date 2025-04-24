//left wheels
#define LW_f 2
#define LW_i 4
#define LW_EN 3
//right wheels
#define RW_f 6
#define RW_i 7
#define RW_EN 5
//line sensor
//#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
//#define S5 A4

int defaultSpeed = 200;

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
    //pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    pinMode(S3, INPUT);
    pinMode(S4, INPUT);
    //pinMode(S5, INPUT);
}

void loop()
{
    //int s1 = digitalRead(S1);
    int s2 = digitalRead(S2);
    int s3 = digitalRead(S3);
    int s4 = digitalRead(S4);
    //int s5 = digitalRead(S5);

    if(s3 == LOW)
        stop();
        //backward();
    else
    {
        if(s2 == HIGH && s4 == HIGH)
            forward();
        else
        {
            if(s2 == HIGH && s4 == LOW)
                turnLeft();
            else if(s2 == LOW && s4 == HIGH)
                turnRight();
            else
                stop();
        }
    }
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
    digitalWrite(LB_f, lb_f);
    digitalWrite(LB_f, lb_f);
    analogWrite(LF_EN, lf_en); //speed
    //right wheels
    digitalWrite(LB_f, lb_f);
    digitalWrite(LB_f, lb_f);
    analogWrite(RF_EN, rf_en); //speed
}
