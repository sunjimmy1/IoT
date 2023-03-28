#include <Arduino.h>
#include "main.hpp"

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  //Call .begin() to configure the IMUs
  if (myIMU.begin() != 0) {
    Serial.println("Device error");
  }
  Serial.println("Setting up...");
  sparkStart(defaultBaudRate,DEVEUI,APPEUI,APPKEY);
  mQjoin();
}

void loop()
{
    Serial.println("Main loop starting...\n");
    // Temperature
    float temperature = sparkTemp(pinTempSensor);
    // mQsend(4,lppTemp(temperature));
    // delay(delayTime);
    long sound = readSound(pinSoundSensor);
    // mQsend(4,lppSound(sound));
    // delay(delayTime);
    int distance = readDistance(ultrasonic);
    // mQsend(3,lppDistance(distance));
    mQsend(11, lppTemp(temperature) + lppSound(sound) + lppDistance(distance) );
    delay(delayTime);
    readAccel(myIMU, accel[0], accel[1], accel[2]);
    readGyro(myIMU, gyro[0], gyro[1], gyro[2]);
    mQsend(8, lppGyro(gyro));
    delay(delayTime);
    mQsend(8, lppAccel(accel));
    delay(delayTime);
}
