//*** ----> LoRa parameters here:
//PLEASE CREATE YOUR OWN PARAMETERS USING A RANDOM HEX GENERATOR
#define  DEVEUI "66d56b1f2dfa843c" // Put your 16 hex char here
#define  APPEUI "132e00af91f22f65" // Put your 16 hex char here
#define  APPKEY "ba959087a225719855ff7ae69463b9f2" // Put your 32 hex char here
//*** <---- END LoRa parameters

//Include libraries 
#include "mQsparkT.h"
#include <Arduino.h>
//*** Define in which port the temp sensor is plugged in ---->
int tempSensorPin = A3;      // Grove - Temperature Sensor connects to port closest to USB port

// globals
int DELAY_MS = 15000;

// Set device baud rate
long defaultBaudRate = 19200;
int joined = 0;

void setup() {
  Serial.println("Setting up...");
  sparkStart(defaultBaudRate,DEVEUI,APPEUI,APPKEY);
  mQjoin();
}

void loop() {
    // main loop where each connected sensors is read, data is transmitted after each read
    // uncomment the lines you want to run
    Serial.println("Main loop starting...\n");

    // Temperature
    float temperature = sparkTemp(tempSensorPin);
    mQsend(4,lppTemp(temperature));

    delay(DELAY_MS);
}
