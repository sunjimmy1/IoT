#include <stdint.h>
#include <Arduino.h>
#include <math.h>
#include "Ultrasonic.h"
#include <LSM6DS3.h>
#include <Wire.h>

/*!
* \Method to initialize the MQSpark. Will initialize serial connection between host and the board and between board and module
* Will set up OTAA credentials as well as power up muRata module.
*
* \param[IN]  baudrate - Baudrate for the serial connections. Use 19200.
* \param[IN]  deveui - a unique 8 byte device identifier.
* \param[IN]  appeui - an application identifier 8 bytes. It's recommended that this value is unique, but it's not a requirement.
* \param[IN]  appkey - ann application key
*/
void sparkStart(long baudrate, String deveui,String appeui, String appkey);

/*!
* \Method to initialize JoinProcedure. The mQSpark will initialize join from sending JoinRequest on a random channel from channel octet 0.
* If the JoinSuccessful the function will return. If not, the mQSpark will keep trying to join on a random channel from every channel octec(0-9)
*/
void mQjoin();

/*!
* \Method to send an AT command to the muRata module.
*/
void send_LoRa_Command(String cmd);

/*!
* \Method to read data from the muRata module (uses serial connection) Will the output to the host controller
*/
void read_data_from_LoRa_Mod();

/*!
* \Method to send data over LoRaWAN.
* \param[IN]  pSize - message size in bytes (how many bytes is your message)
* \param[IN]  rawdata - a HEX string what data to send
*/
void mQsend(int pSize, String rawdata);

/*!
* \Method to read a temperature sensor value on port tempSensorPin.
* \param[IN]  soundSensorPin - a pin to which the sensor is connected
* \retval     result - a reading of the temperature in C
*/
float sparkTemp(int tempSensorPin);

/*!
* \Method to convert temperature sensor reading to the Cayenne LPP formatted string.
* \param[IN]  t - temperature in celcius
* \retval     result - a Cayenne LPP formatted HEX string with the temperature reading
*/
String lppTemp(float t);

/*!
* \Blink LED on board n times.
* \param[IN]  n - how many times blink green LED
*/
void blinky(uint8_t n);

/*
* \Read sound from sound sensor
* \param[IN] pinSoundSensor - the pin the sound sensor is connected to
* \retval The loundness the sensor detects
*/
long readSound(int pinSoundSensor);

String lppSound(long s);

int readDistance(Ultrasonic Ultrasonic);

String lppDistance(int dist);

void readAccel(LSM6DS3 sensor, float &x, float &y, float &z);

void readGyro(LSM6DS3 sensor, float &x, float &y, float &z);

String lppAccel(float accel[3]);

String lppGyro(float gyro[3]);