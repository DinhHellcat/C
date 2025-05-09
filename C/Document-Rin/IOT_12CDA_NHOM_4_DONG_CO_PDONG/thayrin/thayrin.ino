#include <AFMotor.h>
byte blue = 0; //Biến nhận dữ liệu qua bluetooth
AF_DCMotor motor1(1); //Khai báo động cơ 1
AF_DCMotor motor2(2); //Khai báo động cơ 2
AF_DCMotor motor3(3); //Khai báo động cơ 3
AF_DCMotor motor4(4); //Khai báo động cơ 4
void setup() {
    Serial.begin(9600); //Mở cổng Serial
    Serial3.begin(9600); //Mở cổng Serial 3
    motor1.setSpeed(255);//Đặt tốc độ động cơ
    motor2.setSpeed(255);
    motor3.setSpeed(255);
    motor4.setSpeed(255);
    pinMode(30, OUTPUT);//pinMode các chân đèn
    pinMode(31, OUTPUT);
    pinMode(32,OUTPUT);
    pinMode(33,OUTPUT);
}

void loop() 
{
  digitalWrite(30,HIGH);//Mở đèn
  digitalWrite(31,HIGH);
  digitalWrite(32,HIGH);
  digitalWrite(33,HIGH);
  if ( Serial3.available() > 0 )
  {
      blue = Serial3.read();
      Serial3.println(blue);
  }
  if ( blue == 1) //tien
  {
      Serial.println("MOVE FORWARD!");
      motor1.run(FORWARD);      // động cơ tiến 
      motor2.run(BACKWARD);
      motor3.run(FORWARD);        
      motor4.run(BACKWARD);
      blue=0;    
      delay(5);
  }
  if (blue==2) //lui
  {
      Serial.println("MOVE BACKWARD!");
      motor1.run(BACKWARD);     // động cơ lùi  
      motor2.run(FORWARD);       
      motor3.run(BACKWARD);       
      motor4.run(FORWARD);  
      blue=0;      
      delay(5);  
  }
  if (blue==3) //xoaytrai
  {
      Serial.println("ROTATE LEFT!");
      motor1.run(BACKWARD);//Để xoay trái: 2 bánh trái lùi, 2 bánh phải tiến
      motor2.run(BACKWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      blue=0;
      delay(5);
  }
  if (blue==4) //xoay phai
  {
      Serial.println("ROTATE RIGHT!");
      motor1.run(FORWARD);//Ngược lại với xoay trái
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      blue=0;
      delay(5);
  }
  if (blue==6)//Dung dong co
  {
      Serial.println("STOP!");
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
      blue=0;
      delay(5);
  }
  delay(5);
}
//hvnhuy
