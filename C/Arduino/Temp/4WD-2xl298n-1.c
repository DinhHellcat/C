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
    //Bánh trước
    digitalWrite(LF_f, HIGH);
    digitalWrite(LF_i, LOW);
    digitalWrite(RF_f, HIGH);
    digitalWrite(RF_i, LOW);

    analogWrite(LF_EN, defaultSpeed);
    analogWrite(RF_EN, defaultSpeed);
    //Bánh sau
    digitalWrite(LB_f, HIGH);
    digitalWrite(LB_i, LOW);
    digitalWrite(RB_f, HIGH);
    digitalWrite(RB_i, LOW);

    analogWrite(LB_EN, defaultSpeed);
    analogWrite(RB_EN, defaultSpeed);
}
void backward()
{
    //Bánh trước
    digitalWrite(LF_f, LOW);
    digitalWrite(LF_i, HIGH);
    digitalWrite(RF_f, LOW);
    digitalWrite(RF_i, HIGH);

    analogWrite(LF_EN, defaultSpeed);
    analogWrite(RF_EN, defaultSpeed);
    //Bánh sau
    digitalWrite(LB_f, LOW);
    digitalWrite(LB_i, HIGH);
    digitalWrite(RB_f, LOW);
    digitalWrite(RB_i, HIGH);

    analogWrite(LB_EN, defaultSpeed);
    analogWrite(RB_EN, defaultSpeed);
}
void stop()
{
    //Bánh trước
    digitalWrite(LF_f, LOW);
    digitalWrite(LF_i, LOW);
    digitalWrite(RF_f, LOW);
    digitalWrite(RF_i, LOW);

    analogWrite(LF_EN, 0);
    analogWrite(RF_EN, 0);
    //Bánh sau
    digitalWrite(LB_f, LOW);
    digitalWrite(LB_i, LOW);
    digitalWrite(RB_f, LOW);
    digitalWrite(RB_i, LOW);

    analogWrite(LB_EN, 0);
    analogWrite(RB_EN, 0);
}
