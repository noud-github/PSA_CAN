// CAN Receive Example
//

#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adapted_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string


// huzzah32 MOSI 18;MISO 19;SCK 5;SS 2**
// huzzah32 A5 CS, SCK SCK, MO SI, MI SO  


#define CAN0_INT 13                              // Set INT to pin 13
MCP_CAN CAN0(SS);                               // Set CS to pin A5

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

volatile unsigned char Flag_Recv = 0;


void setup()
{
  Serial.begin(115200);
   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT_PULLUP);                            // Configuring pin for /INT input
 
  attachInterrupt(digitalPinToInterrupt(CAN0_INT), MCP2515_ISR ,FALLING);
  
  Serial.println("MCP2515 Library Receive Example...");
}

void MCP2515_ISR()
{
    Flag_Recv = 1;
    
}


void loop()
{
    
  if(Flag_Recv)                         // If CAN0_INT pin is low, read receive buffer
  {
    Flag_Recv = 0;
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX DLC: %1d  Data:", rxId, len);
     
    Serial.print(msgString);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print(msgString);
    
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
      display.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
        display.print(msgString);
      }
    }
        
    Serial.println();
    display.display();
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
