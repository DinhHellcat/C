// Bánh trước
// Trái (Left Front)
#define LF_f  13
#define LF_i  12
#define LF_EN 11
//Phải (Right Front)
#define RF_f  9
#define RF_i  8
#define RF_EN 10
// Bánh sau
// Trái (Left Behind)
#define LB_f  7
#define LB_i  6
#define LB_EN 5
// Phải (Right Behind)
#define RB_f  4
#define RB_i  2
#define RB_EN 3

int defaultSpeed = 200;

void setup()
{
    //Bánh trước
    pinMode(LF_f, OUTPUT);
    pinMode(LF_i, OUTPUT);
    pinMode(LF_EN, OUTPUT);

    pinMode(RF_f, OUTPUT);
    pinMode(RF_i, OUTPUT);
    pinMode(RF_EN, OUTPUT);
    //Bánh sau
    pinMode(LB_f, OUTPUT);
    pinMode(LB_i, OUTPUT);
    pinMode(LB_EN, OUTPUT);

    pinMode(RB_f, OUTPUT);
    pinMode(RB_i, OUTPUT);
    pinMode(RB_EN, OUTPUT);
}

void loop()
{
    forward();
    delay(2000);

    stop();
    delay(2000);

    backward();
    delay(2000);
}

void forward()
{
    setMotor(HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, defaultSpeed, defaultSpeed, defaultSpeed, defaultSpeed);
}

void backward()
{
    setMotor(LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, defaultSpeed, defaultSpeed, defaultSpeed, defaultSpeed);
}

void stop()
{
    setMotor(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, 0, 0, 0, 0);
}

void setMotor(int lf_f, int lf_i, int rf_f, int rf_i, int lb_f, int lb_i, int rb_f, int rb_i, int lf_en, int rf_en, int lb_en, int rb_en)
{
    //Bánh trước
    digitalWrite(LF_f, lf_f);
    digitalWrite(LF_i, lf_i);
    digitalWrite(RF_f, rf_f);
    digitalWrite(RF_i, rf_i);

    //Bánh sau
    digitalWrite(LB_f, lb_f);
    digitalWrite(LB_i, lb_i);
    digitalWrite(RB_f, rb_f);
    digitalWrite(RB_i, rb_i);

    //Speed
    analogWrite(LF_EN, lf_en);
    analogWrite(RF_EN, rf_en);
    analogWrite(LB_EN, lb_en);
    analogWrite(RB_EN, rb_en);
}
