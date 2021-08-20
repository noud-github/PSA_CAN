// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10);     // Set CS to pin 10

void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

unsigned char txBuf[7] = {0b00000000,0b00000000,0b00101010,0b11111111,0b11111111,0b11111101,0b11111111};

void loop()
{
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  for(byte j = 0; j<=0b111; j++){
    txBuf[1] = (txBuf[1] & 0b00011111) + (j  << 5) ;
    //txBuf[3] = (txBuf[3] & 0b00011111) + (j  << 5) ;
    //txBuf[3] = (txBuf[3] & 0b11100011) + (j  << 2) ;
    //txBuf[4] = (txBuf[4] & 0b00011111) + (j  << 5) ;
    //txBuf[4] = (txBuf[4] & 0b11100011) + (j  << 2) ;
    //txBuf[5] = (txBuf[5] & 0b00011111) + (j  << 5) ;
    //txBuf[5] = (txBuf[5] & 0b11100011) + (j  << 2) ;
    byte sndStat = CAN0.sendMsgBuf(0x0E0, 0, 7, txBuf);
    if(sndStat == CAN_OK){
        //Serial.println("Message Sent Successfully!");
    } else {
        Serial.println("Error Sending Message...");
    }
    delay(100);   // send data per 100ms
    

    
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
