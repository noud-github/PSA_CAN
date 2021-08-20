// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>
#include <PSA_CAN.h>

/*
typedef struct {
  byte LeftFront;
  byte CenterFront;
  byte RightFront;
  byte LeftRear;
  byte CenterRear;
  byte RightRear;
  bool Show;
} PDC;
*/

MCP_CAN CAN0(10);     // Set CS to pin 10
msgId_0E1 parktronic;

void setup()
{
  Serial.begin(115200);
  pinMode(3,INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  //digitalWrite(3, HIGH); //turn pullup resistor on
  //digitalWrite(4, HIGH); //turn pullup resistor on

  // Initialize MCP2515 running at 16MHz with a baudrate of 125kb/s and the masks and filters disabled.
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
}

void loop()
{
  Check_if_Button_Pressed();
  CAN_Sent_0E1_maintain();
  if (parktronic.getShowWindowValue()){ 
    PDC_Action_maintain();
  }

 //Serial.println (parktronic.Period);
}

void CAN_Sent_0E1_maintain()
{
  
  //static const unsigned long REFRESH_INTERVAL = 100; // ms
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= parktronic.Period) 
  {
    lastRefreshTime += parktronic.Period;
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
    
   if (parktronic.getShowWindowValue()){
      parktronic.getMsgBuf(txBuf_0E1); // get Msg buffer
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
  static int  switchState_A= LOW;
  static int  switchReading_A=LOW;
  static int prevswitchState_A= LOW;
  static int  switchReading_B=LOW;
  static int  switchState_B=LOW;
  static int prevswitchState_B= LOW;
  static unsigned long lastDebounceTime_A = 0;  // the last time the output pin was toggled
  static unsigned long lastDebounceTime_B = 0;  // the last time the output pin was toggled
  static unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

  
  switchReading_A = digitalRead(3);
  if (switchReading_A != prevswitchState_A){
      //Serial.println("Pressed_A");
      lastDebounceTime_A = millis();
      //parktronic.setShowWindowValue ( !((bool)parktronic.getShowWindowValue()));
     
  }

  if ((millis() - lastDebounceTime_A) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (switchReading_A != switchState_A) {
      switchState_A = switchReading_A;
     
        
      // only toggle the LED if the new button state is HIGH
      if (switchState_A  == LOW) {
         Serial.println("Pressed_A");
        parktronic.setShowWindowValue ( !((bool)parktronic.getShowWindowValue()));
      }
    }
  }
  prevswitchState_A = switchReading_A;

  switchReading_B = digitalRead(4);
  if (switchReading_B != prevswitchState_B){
      //Serial.println("Pressed_B");
      lastDebounceTime_B = millis();
     
     
  }

  if ((millis() - lastDebounceTime_B) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (switchReading_B != switchState_B) {
      switchState_B = switchReading_B;
      
        
      // only toggle the LED if the new button state is HIGH
      if (switchState_B  == LOW) {
        Serial.println("Pressed_B");  
      }
    }
  }
  prevswitchState_B = switchReading_B;
  
}

void PDC_Action_maintain()
{
  
  static const unsigned long REFRESH_INTERVAL = 750; // ms
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
    parktronic.setRearCenterBits(abs(7));
    parktronic.setRearLeftBits(abs(7));
    parktronic.setRearRightBits(abs(7));
    parktronic.setFrontCenterBits(abs(i));
    parktronic.setFrontLeftBits(abs(j));
    parktronic.setFrontRightBits(abs(k));
 }else{
    parktronic.setRearCenterBits(abs(i));
    parktronic.setRearLeftBits(abs(j));
    parktronic.setRearRightBits(abs(k));
    parktronic.setFrontCenterBits(abs(7));
    parktronic.setFrontLeftBits(abs(7));
    parktronic.setFrontRightBits(abs(7));
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
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
