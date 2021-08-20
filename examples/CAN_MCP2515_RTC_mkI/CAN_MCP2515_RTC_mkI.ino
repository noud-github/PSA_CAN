// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
// CAN Send
#include <Wire.h>
#include "RTClib.h"
#include <mcp_can.h>
#include <SPI.h>
#include <EEPROM.h>
#include "PSA_CAN.h"

int addr = 0;
//char buffer[20]; // Allocate some space for the buffer
//char inChar=-1; // Where to store the character read
//byte index = 0; // Index into array; where to store the character
//char Data[20]; // Allocate some space for the string
bool off = false;

MCP_CAN CAN0(10);     // Set CS to pin 10
msgId_276 BSI_Clock;

RTC_DS3231 rtc;
const int sensorPin = A0;
// room temperature in Celsius
//const float baselineTemp = 20.0;


void setup () {

  Serial.begin(115200);
  //memset(buffer, '\0', 20); //reset buffer

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  START_INIT:
    // Initialize MCP2515 running at 16MHz with a baudrate of 125kb/s and the masks and filters disabled.
    if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK)                   // init can bus : baudrate = 500k
    {
        Serial.println("MCP2515 Initialized Successfully!");
    }
    else
    {
        Serial.println("Error Initializing MCP2515...");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

   byte value = EEPROM.read(addr);
   if (bool((value & 0b00000010) >> 1)) {
      Serial.println("Reset: On ");
   } else {
      Serial.println("Reset: Off ");
   }

   if (bool((value & 0b00000001))) {
      Serial.println("24H: On ");
   } else {
      Serial.println("24H: Off ");
   }
  
}

void loop () {
    
    if (MessageIn("reset on")) {
        Serial.write("reset value -> ON\n");
        byte value = EEPROM.read(addr);
        value = ((value & 0b11111101) + 0b10);
        EEPROM.write(addr, value);
    }
    if (MessageIn("reset off")) {
        Serial.write("reset value -> Off\n");
        byte value = EEPROM.read(addr);
        value = (value & 0b11111101);
        EEPROM.write(addr, value);
    }
    if (MessageIn("24h on")) {
        Serial.write("24Hours Notation -> ON\n");
        byte value = EEPROM.read(addr);
        value = ((value & 0b11111110) + 0b1);
        EEPROM.write(addr, value);
    }
    if (MessageIn("24h off")) {
        Serial.write("24Hours Notation -> Off\n");
        byte value = EEPROM.read(addr);
        value = (value & 0b11111110);
        EEPROM.write(addr, value);
    }
    if (MessageIn("eco mode on")) {
        Serial.write("Eco Mode -> On\n");
        off = true;
    }
    if (MessageIn("eco mode off")) {
        Serial.write("Eco Mode -> Off\n");
        off = false;
    }
    
    if (!off) {
      CAN_Sent_276_maintain();
      CAN_Sent_0F6_maintain();
    }
}

bool MessageIn(char* This) {
    static char rx_buffer[20]; // Allocate some space for the buffer
    char inChar=-1; // Where to store the character read
    static byte index = 0; // Index into array; where to store the character
    static char rx_string[20]; // Allocate some space for the string
    
    if (Serial.available() > 0) // Don't read unless you know there is data
    {
        char endMarker = '\n';
        inChar = Serial.read(); // Read a character
        if (inChar != endMarker) // check for return
        {
          if(index < 20) // One less than the size of the array
          {
              rx_buffer[index] = inChar; // Store it
              index++; // Increment where to write next  
          }
       } else {
          sprintf(rx_string , rx_buffer); //copy buffer to output
          memset(rx_buffer, '\0', 20); //reset buffer
          index = 0; // reset index
          //Serial.println (Data);
       }
    }
     
    if ((strcmp(rx_string,This)  == 0)) 
    {
        memset(rx_string, 0, 20); //reset buffer
        return(true);
    } else {
        return(false);
    }
}


void CAN_Sent_276_maintain()
{
  
  static unsigned long lastRefreshTime = 0;
  
  if(millis() - lastRefreshTime >= BSI_Clock.Period) 
  {
    lastRefreshTime += BSI_Clock.Period;
                CAN_Sent_276();
  } else { 
    if (millis() - lastRefreshTime < 0){
          lastRefreshTime = 0;
          CAN_Sent_276();
    }
  }
}

void CAN_Sent_276(){
   //276 Sent on: period 1000
   
   BSI_Clock.setTime(rtc.now());
   BSI_Clock.setShow24HValue(EEPROM.read(addr)& 0b00000001);
   BSI_Clock.setResetValue((EEPROM.read(addr)& 0b00000010) >> 1);
   
   unsigned char txBuf_276[7];
   BSI_Clock.getMsgBuf(txBuf_276);
   byte sndStat = CAN0.sendMsgBuf(0x276, 0, 7, txBuf_276);
   if(sndStat == CAN_OK){
        //Serial.println("Message Sent Successfully!");
   } else {
        Serial.print(sndStat);
        Serial.println(" Error Sending Message...");
   }
}

void CAN_Sent_0F6_maintain()
{
  //Sent on: period 500
  static const unsigned long REFRESH_INTERVAL_0F6 = 500; // ms
  static unsigned long lastRefreshTime_0F6 = 0;
  
  if(millis() - lastRefreshTime_0F6 >= REFRESH_INTERVAL_0F6) 
  {
    lastRefreshTime_0F6 += REFRESH_INTERVAL_0F6;
    CAN_Sent_0F6();
  } else { 
    if (millis() - lastRefreshTime_0F6 < 0){
          lastRefreshTime_0F6 = 0;
          CAN_Sent_0F6();
         
    }
  }
}

void CAN_Sent_0F6()
{
    int sensorVal = analogRead(sensorPin);

  // send the 10-bit sensor value out the serial port
  //Serial.print("sensor Value: ");
  //Serial.print(sensorVal);

  // convert the ADC reading to voltage
  float voltage = (sensorVal / 1024.0) * 5.0;

  // Send the voltage level out the Serial port
  //Serial.print(", Volts: ");
  //Serial.print(voltage);

  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((voltage - 500 mV) times 100)
  int tx_Temperature = round(2*(((voltage - .5) * 100)) + 79);
  //Serial.print(", TX_Temp: ");
  //Serial.print(tx_Temperature);
  
  //Serial.print(", degrees C: ");
  //int temperature = round((voltage - .5) * 100);
  //Serial.println(temperature);
  
  
  //0F6
  unsigned char txBuf_0F6[8] = {0b1001110,0b00000000,0b00000000,0b00000000,0b00000000,0b10001110,0b00000000,0b00000000};
  
  txBuf_0F6[6] = (tx_Temperature) ;
  byte sndStat = CAN0.sendMsgBuf(0x0F6, 0, 8, txBuf_0F6);
    if(sndStat == CAN_OK){
    //    //Serial.println("Message Sent Successfully! 0F6");
    } else {
        Serial.print("Error Sending Message 0F6...");
        Serial.println(sndStat);
    }

  
}


