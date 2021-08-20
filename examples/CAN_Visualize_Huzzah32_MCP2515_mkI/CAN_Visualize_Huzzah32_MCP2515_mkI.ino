/*********************************************************************
*/
#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PSA_CAN.h>
#include "Display7215.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Display7215 Screen(&display);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


char msgString[128];        
msgId_0E1 Parktronic;
msgId_276 BSI_time;
msgId_0F6 Temperature;

unsigned long lastMessage = 0;




long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
      

#define CAN0_INT 36                              // Set INT to pin 36
MCP_CAN CAN0(A5);                               // Set CS to pin A5



/*
byte Display_PDC = 0;
byte Front_Center_Value = 0;
byte Front_Left_Value = 0;
byte Front_Right_Value = 0;
byte Rear_Center_Value = 0;
byte Rear_Left_Value = 0;
byte Rear_Right_Value = 0;
int hours = 0;
int minutes = 0;
int temperature = 30;
bool DataChanged = 1;
bool Show_24h = true;
byte Show_parktronic_value = 0;
*/
void setup()   {                
  //Wire.setClock(3400000);
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  
  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK){
    Serial.println("MCP2515 Initialized Successfully!");
  }else{
    Serial.println("Error Initializing MCP2515...");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Error Initializing MCP2515...");
    display.display();
  }
  // Set operation mode to normal so the MCP2515 sends acks to received data.
  if(CAN0.setMode(MCP_NORMAL) == CAN_OK){
     Serial.println("MCP2515 setMode Successfully!");
     display.clearDisplay();
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(0,0);
     display.print("00:00");
     display.setCursor(110,0);
     display.print("-00");
     display.display();
  }else{
    Serial.println("Error setMode MCP2515...");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Error setMode MCP2515...");
    display.display();
    
   }
  pinMode(CAN0_INT, INPUT_PULLUP);                            // Configuring pin for /INT input

  //attachInterrupt(digitalPinToInterrupt(CAN0_INT), ISR,FALLING);
  
  Serial.println("CAN Visualize Demo By 80\"");

   
 
  
    
}

void loop() {

  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX DLC: %1d  Data:", rxId, len);
    
    if (rxId == 0x276){
        BSI_time.setMsgBuf(rxBuf);
    }
    if (rxId == 0x0E1){
          Parktronic.setMsgBuf(rxBuf);
    }
    if (rxId == 0x0F6){
      //Temprature
      Temperature.setMsgBuf(rxBuf);
    }

    
    Serial.print(msgString);
    
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
      //display.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
        //display.print(msgString);
      }
    }
    if (BSI_time.MessageChanged or Parktronic.MessageChanged or Temperature.MessageChanged ) {
      display.clearDisplay();
      Screen.DrawClock(BSI_time);
      Screen.DrawTemperature(Temperature);
      Screen.DrawPDC(Parktronic);
      display.display();
      

      Serial.print(" refreshed ");
    }
    
    
    
    Serial.println();
    
}
   
}
