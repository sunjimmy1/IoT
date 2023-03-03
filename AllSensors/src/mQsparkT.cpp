#include "mQsparkT.hpp"

void blinky(uint8_t n)
{
    while (n > 0)
    {
        digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(200);             // wait for 200 mil
        digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW
        delay(200);             // wait for 200 mil
        n--;
    }
}

//created this function to make it clear when a function is printing to the console versus to the serial port to Murata (Serial1)
static void print_to_console(String message)
{
    Serial.println(message);
}

void read_data_from_LoRa_Mod()
{
    if (Serial1.available())
    {
        String inByte = Serial1.readString();
        Serial.println(inByte);
    }
}

void send_LoRa_Command(String cmd)
{
    print_to_console("Sending: " + cmd);
    Serial1.print(cmd + "\r");
}

void sparkStart(long baudrate, String deveui, String appeui, String appkey)
{
    Serial.begin(baudrate);
    Serial1.begin(baudrate);
    delay(2000);
    print_to_console("Serial initialised");

    pinMode(13, OUTPUT); // Initialize LED port
    pinMode(9, INPUT);
    pinMode(7, INPUT_PULLUP);
    blinky(2);

    pinMode(6, OUTPUT); //Enable power to the Murata
    digitalWrite(6, HIGH);

    print_to_console("Murata Powered up");

    pinMode(5, OUTPUT); //Disable reset pin
    digitalWrite(5, HIGH);
    delay(2000);
    read_data_from_LoRa_Mod();

    /***
    send_LoRa_Command("AT+VER?");   //Firmware Version
    delay(500);
    read_data_from_LoRa_Mod();
    ***/
    
    //Begin configuring LoRaWAN Parameters
    send_LoRa_Command("AT+MODE=1"); 
    delay(250);
    read_data_from_LoRa_Mod();

    send_LoRa_Command("AT+DEVEUI=" + deveui);
    delay(250);
    read_data_from_LoRa_Mod();

    send_LoRa_Command("AT+APPKEY=" + appkey);
    delay(250);
    read_data_from_LoRa_Mod();

    send_LoRa_Command("AT+APPEUI=" + appeui);
    delay(250);
    read_data_from_LoRa_Mod();

    send_LoRa_Command("AT+DFORMAT=1"); //Message formatting
    delay(250);
    read_data_from_LoRa_Mod();

    send_LoRa_Command("AT+BAND=8"); //Set band to US915
    delay(250);
    read_data_from_LoRa_Mod();
    digitalWrite(13, HIGH);
}

void mQjoin() {
    Serial.println("Starting mQjoin()\r");
    // counter how many retries sent
    uint8_t joinTries = 0;
    // flag if device has joined or not
    bool joined = false;
    send_LoRa_Command("AT+JOIN"); //Join network
    delay(500);
    // will read "+OK"
    read_data_from_LoRa_Mod();
    while (!joined)
    {
        // serial will be available after 9 retries (each cahnnel block)
        if (Serial1.available())
        {
            String inByte = Serial1.readString();
            //print the module response
            Serial.println(inByte.substring(0, 10));
            // check if the response has event 1,1 - means the device has joined
            if (inByte.substring(0, 10) == "+EVENT=1,1")
            {
                //set flag to true, will indicate join is successful and exit the loop
                joined = true;
                Serial.println("\nJoined!\n");
            }
            else
            {
                Serial.println("Join channel not found, start again\r");
                joinTries = 0;
                send_LoRa_Command("AT+JOIN"); //Join network
                delay(500);
                // will read "+OK"
                read_data_from_LoRa_Mod();
            }
        }
        else
        {
            // join is still ongoing, print which block is now
            Serial.println("Join in process. Channel Block " + String(joinTries));
            joinTries++;
            // delay for 6 seconds and extra 400 (transmission ~300 + serial comm 50 + 50 downlink message processing)
            delay(6400);
        }
    }
}

void mQsend(int pSize, String rawdata)
{
    send_LoRa_Command("AT+UTX " + String(pSize));
    send_LoRa_Command(rawdata);
    delay(3000);
    // read if there was RX data
    read_data_from_LoRa_Mod();
    blinky(2);
}

// *** Grove sensor functions ****

// Temperature code from http://wiki.seeedstudio.com/Grove-Temperature_Sensor_V1.2/#software
// This function samples the temperature sensor and returns a temperature value
float sparkTemp(int tempSensorPin) // Grove Temp Sensor
{
    const uint16_t B = 4275;    // B value of the thermistor
    const uint32_t R0 = 100000; // R0 = 100k
    int a = 0;

    a = analogRead(tempSensorPin);
    float R = 1023.0 / a - 1.0;
    R = R0 * R;
    float temp = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to temperature via datasheet

    return temp;
}

// To send temperature data
String lppTemp(float t)
{ //converts sensor reading to LPP format
    //convert to hex and LPP
    int16_t temp = int16_t(t * 10);
    char tempHex[5]={0};
    sprintf(tempHex, "%.4X", temp);
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("\n");
    return "0067" + String(tempHex); //adds channel and data type header to string
}

long readSound(int pinSoundSensor)
{
  long sum = 0;
  for (int i = 0; i < 32; i++)
  {
    sum += analogRead(pinSoundSensor);
  }
  sum >>= 5;
  return sum;
}

// To send Sound data
String lppSound(long s)
{ //converts sensor reading to LPP format
    //convert to hex and LPP
    int16_t sound = int16_t(s * 100);
    char soundHex[5]={0};
    sprintf(soundHex, "%.4X", sound);
    Serial.print("Sound Level: ");
    Serial.print(s);
    Serial.print("\n");
    return "0002" + String(soundHex); //adds channel and data type header to string
}

int readDistance(Ultrasonic ultrasonic)
{
  return ultrasonic.MeasureInInches();
}

String lppDistance(int dist){
    //converts sensor reading to LPP format
    //convert to hex and LPP
    int16_t distance = int16_t(dist);
    char distHex[3]={0};
    sprintf(distHex, "%.2X", distance);
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.print("\n");
    return "0000" + String(distHex); //adds channel and data type header to string
}


