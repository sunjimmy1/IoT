#include <Arduino.h>
#include "main.hpp"

void setup()
{
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
    mQsend(11, lppTemp(temperature) + lppSound(sound) + lppDistance(distance));
    delay(delayTime);

}
