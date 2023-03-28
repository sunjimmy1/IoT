#include <math.h>
#include <Wire.h>
#include "LSM6DS3.h"
#include "mQsparkT.hpp"

//*** ----> LoRa parameters here:
//PLEASE CREATE YOUR OWN PARAMETERS USING A RANDOM HEX GENERATOR
#define  DEVEUI "66d56b1f2dfa843c" // Put your 16 hex char here
#define  APPEUI "132e00af91f22f65" // Put your 16 hex char here
#define  APPKEY "ba959087a225719855ff7ae69463b9f2" // Put your 32 hex char here
//*** <---- END LoRa parameters

const int B = 4275;           // B value of the thermistor
const int R0 = 100000;        // R0 = 100k
const int pinTempSensor = A5; 
const int pinSoundSensor = A0;
const int delayTime = 2500;
Ultrasonic ultrasonic(4);
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
float gyro[3], accel[3];
long defaultBaudRate = 19200;
int joined = 0;

#if defined(ARDUINO_ARCH_AVR)
#define debug Serial
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
#define debug SerialUSB
#else
#define debug Serial
#endif