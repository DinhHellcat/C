#include <Servo.h>    
Servo nhom3_Servo ;   
void setup() 
{ 
    nhom3_Servo.attach(9); 
}
 
void loop() 
{ 
    nhom3_Servo.write(0);      
    delay(1000);                
  nhom3_Servo.write(90);        
    delay(1000);                
  nhom3_Servo.write(180);       
    delay(1000);                 
}
