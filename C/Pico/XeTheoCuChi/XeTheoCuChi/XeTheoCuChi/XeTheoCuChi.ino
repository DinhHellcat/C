#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#define RX_PIN                           3
#define TX_PIN                           2

SoftwareSerial ZigbeeSerial(RX_PIN, TX_PIN);

void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  ZigbeeSerial.begin(9600);
  while (!ZigbeeSerial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax, ay;
  ax = a.acceleration.x;
  ay = a.acceleration.y;

  /* Print out the values */
  Serial.print("Acceleration: ");
  Serial.print(", X: ");
  Serial.print(ax);
  Serial.print(", Y: ");
  Serial.println(ay);

  delay(200);

  if(ax > 3.5)
  {
    ZigbeeSerial.print('B'); //Right
    Serial.println('B');
  }
  else if (ax < -3.5)
  {
    ZigbeeSerial.print('C'); //Left
    Serial.println('C');
  }
  else
  {
    ZigbeeSerial.print('A'); //Straight
    Serial.println('A');
  }

  if(ay > 3.5)
  {
    ZigbeeSerial.print('D'); //Forward
    Serial.println('D');
  }
  else if (ay < -3.5)
  {
    ZigbeeSerial.print('E'); //Stop
    Serial.println('E');
  }
  else
  {
    ZigbeeSerial.print('F'); //Backward
    Serial.println('F');
  }
}