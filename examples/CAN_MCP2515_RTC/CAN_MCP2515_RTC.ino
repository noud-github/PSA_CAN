// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
// CAN Send
#include <Wire.h>
#include "RTClib.h"
#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);     // Set CS to pin 10

RTC_DS3231 rtc;
const int sensorPin = A0;
// room temperature in Celsius
const float baselineTemp = 20.0;


void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(115200);


  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

}

void loop () {

    int sensorVal = analogRead(sensorPin);

  // send the 10-bit sensor value out the serial port
  Serial.print("sensor Value: ");
  Serial.print(sensorVal);

  // convert the ADC reading to voltage
  float voltage = (sensorVal / 1024.0) * 5.0;

  // Send the voltage level out the Serial port
  Serial.print(", Volts: ");
  Serial.print(voltage);

  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((voltage - 500 mV) times 100)
  int tx_Temperature = round(2*(((voltage - .5) * 100) + 39.5));
  Serial.print(", TX_Temp: ");
  Serial.print(tx_Temperature);
  
  Serial.print(", degrees C: ");
  int temperature = round((voltage - .5) * 100);
  Serial.println(temperature);
  //0F6
  //unsigned char txBuf_0f6[8] = {0b10000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00011011,0b00010000,0b00010000};
  
   // display on: period 100
   //unsigned char txBuf_167[8] = {000001001, 0000110, 00010111, 01110000, 01111111, 11111111, 00000000, 00000000};

    
    DateTime now = rtc.now();
    //276
    unsigned char txBuf[7] = {0b10000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00011011,0b00010000};
    //unsigned char txBuf[7] = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00011011,0b00010000};
    //Year
    txBuf[0] = (txBuf[0] & 0b10000000) + (now.year() -2000) ;
    //Month
    txBuf[1] = (txBuf[1] & 0b11110000) + (now.month()) ;
    //Day
    txBuf[2] = (txBuf[2] & 0b11100000) + (now.day()) ;
    //Hour
    txBuf[3] = (txBuf[3] & 0b11100000) + (now.hour()) ;
    //min
    txBuf[4] = (txBuf[4] & 0b11100000) + (now.minute()) ;

    byte sndStat = CAN0.sendMsgBuf(0x276, 0, 7, txBuf);
    if(sndStat == CAN_OK){
        //Serial.println("Message Sent Successfully!");
    } else {
        Serial.print(txBuf[0]);
        Serial.print(" ");
        Serial.print(txBuf[1]);
        Serial.print(" ");
        Serial.print(txBuf[2]);
        Serial.print(" ");
        Serial.print(txBuf[3]);
        Serial.print(" ");
        Serial.print(txBuf[4]);
        Serial.println("Error Sending Message...");
    }

    

    
    
    delay(1000);
}
