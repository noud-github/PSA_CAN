// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>

typedef struct {
  byte LF;
  byte CF;
  byte RF;
  byte LR;
  byte CR;
  byte RR;
  bool Show;
} PDC;


MCP_CAN CAN0(10);     // Set CS to pin 10
int  switchState_A=0;
int prevswitchState_A= LOW;
int  switchState_B=0;
int prevswitchState_B= LOW;
PDC parktronic = {0b111,0b111,0b111,0b111,0b111,0b111,0};


void setup()
{
  Serial.begin(115200);
  pinMode(3,INPUT);
  pinMode(4, INPUT);
  digitalWrite(3, HIGH); //turn pullup resistor on
  digitalWrite(4, HIGH); //turn pullup resistor on

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

void loop()
{
  Check_if_Button_Pressed();
  CAN_Sent_0E1_maintain();
  if (parktronic.Show){ 
    PDC_Action_maintain();
  }
 
}

void CAN_Sent_0E1_maintain()
{
  
  static const unsigned long REFRESH_INTERVAL = 100; // ms
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) 
  {
    lastRefreshTime += REFRESH_INTERVAL;
                CAN_Sent_0E1();
  } else { 
    if (millis() - lastRefreshTime < 0){
          lastRefreshTime = 0;
          CAN_Sent_0E1();
    }
  }
}



void CAN_Sent_0E1(){
   //0E1
   //Sent on: period 100
   unsigned char txBuf_0E1[7] = {0b00000000,0b00000000,0b00101010,0b00000000,0b00000000,0b00000000,0b11111111};
   static byte Sendframe = 2;
    
   if (parktronic.Show){
      // show parktronic window default is off
      txBuf_0E1[5] = ((txBuf_0E1[5] & 0b11111101) + (1  << 1));
    
      //Sound
      //txBuf[1] = (txBuf[1] & 0b00011111) + (j  << 5) ;
      //rear left
      txBuf_0E1[3] = (txBuf_0E1[3] & 0b00011111) + ((parktronic.LR & 0b111) << 5) ;
      //rear center
      txBuf_0E1[3] = (txBuf_0E1[3] & 0b11100011) + ((parktronic.CR & 0b111) << 2) ;
      //rear right
      txBuf_0E1[4] = (txBuf_0E1[4] & 0b00011111) + ((parktronic.RR & 0b111) << 5) ;
      //front left
      txBuf_0E1[4] = (txBuf_0E1[4] & 0b11100011) + ((parktronic.LF & 0b111) << 2) ;
      //front center
      txBuf_0E1[5] = (txBuf_0E1[5] & 0b00011111) + ((parktronic.CF & 0b111) << 5) ;
      //front right
      txBuf_0E1[5] = (txBuf_0E1[5] & 0b11100011) + ((parktronic.RF & 0b111) << 2) ;
      Sendframe = 2;
   }    
   if (Sendframe > 0){
   byte sndStat = CAN0.sendMsgBuf(0x0E1, 0, 7, txBuf_0E1);
   if(sndStat == CAN_OK){
      //Serial.println("Message Sent Successfully!");
   } else {
      Serial.println("Error Sending Message...");
   }
   Sendframe-- ;
   }
   
}


void Check_if_Button_Pressed(){
  switchState_A = digitalRead(3);
  if (switchState_A == LOW && prevswitchState_A == HIGH){
      Serial.println("Pressed_A");
      parktronic.Show = !parktronic.Show;
  }
  prevswitchState_A = switchState_A;
  switchState_B = digitalRead(4);
  if (switchState_B == LOW && prevswitchState_B == HIGH){
      Serial.println("Pressed_B");
      
  }
  prevswitchState_B = switchState_B;
  
}

void PDC_Action_maintain()
{
  
  static const unsigned long REFRESH_INTERVAL = 500; // ms
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) 
  {
    lastRefreshTime += REFRESH_INTERVAL;
                PDC_Action();
  } else { 
    if (millis() - lastRefreshTime < 0){
          lastRefreshTime = 0;
          PDC_Action();
    }
  }
}

void PDC_Action(){
 static int i = -7;
 static int j = -7;
 static int k = -7;
 static bool front = false;
 if (front){
    parktronic.CR = abs(7);
    parktronic.LR = abs(7);
    parktronic.RR = abs(7);
    parktronic.CF = abs(i);
    parktronic.LF = abs(j);
    parktronic.RF = abs(k);
 }else{
    parktronic.CR = abs(i);
    parktronic.LR = abs(j);
    parktronic.RR = abs(k);
    parktronic.CF = abs(7);
    parktronic.LF = abs(7);
    parktronic.RF = abs(7);
 }
 i = i + random(-3,3);
 j = j + random(-3,3);
 k = k + random(-3,3);

 if (abs(k)+abs(j)+abs(i) > 18)
   front = !front;
 
 if (abs(i) > 7){
  i = 7 ;
 }
 if (abs(j) > 7){
  j = 7 ;
 }
 if (abs(k) > 7){
  k = 7 ;
 }
 
  
 
 //Serial.println (i);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
