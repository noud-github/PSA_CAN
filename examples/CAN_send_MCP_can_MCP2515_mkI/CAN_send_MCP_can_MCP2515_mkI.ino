// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>

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

typedef struct {
  byte byte0;
  byte byte1;
  byte byte2;
  byte byte3;
  byte byte4;
  byte byte5;
  byte byte6;
} DATA_0E1;

class Parktronic {
private:
  DATA_0E1 data;
  
public:
  Parktronic();
  byte getLeftFront();
  byte getCenterFront();
  byte getRightFront();
  byte getLeftRear();
  byte getCenterRear();
  byte getRightRear();
  bool getShowWindow();
  void setLeftFront(byte val);
  void setCenterFront(byte val);
  void setRightFront(byte val);
  void setLeftRear(byte val);
  void setCenterRear(byte val);
  void setRightRear(byte val);
  void setShowWindow(bool val);
  void getMsgBuf(byte *ptr);
};


Parktronic::Parktronic()
{
  this->data = {0b00000000,0b00000000,0b00101010,0b00000000,0b00000000,0b00000000,0b11111111};
}

byte Parktronic::getLeftFront()
{
  return (byte)((this->data.byte4 >> 2) & 0b111);
}

byte Parktronic::getCenterFront()
{
  return (byte)((this->data.byte5 >> 5) & 0b111);
}

byte Parktronic::getRightFront()
{
  return (byte)((this->data.byte5 >> 2) & 0b111);
}

byte Parktronic::getLeftRear()
{
  return (byte)((this->data.byte3 >> 5) & 0b111);
}

byte Parktronic::getCenterRear()
{
  return (byte)((this->data.byte3 >> 2) & 0b111);
}

byte Parktronic::getRightRear()
{
  return (byte)((this->data.byte4 >> 5) & 0b111);
}

bool Parktronic::getShowWindow()
{ 
  return (bool)((this->data.byte5 & 0b00000010) >> 1);
}

void Parktronic::setLeftFront(byte val)
{
  this->data.byte4 = (this->data.byte4 & 0b11100011) + ((val & 0b111) << 2) ;
}

void Parktronic::setCenterFront(byte val)
{
  this->data.byte5 = (this->data.byte5 & 0b00011111) + ((val & 0b111) << 5) ;
}

void Parktronic::setRightFront(byte val)
{
  this->data.byte5 = (this->data.byte5 & 0b11100011) + ((val & 0b111) << 2) ;
}

void Parktronic::setLeftRear(byte val)
{
  this->data.byte3 = (this->data.byte3 & 0b00011111) + ((val & 0b111) << 5) ;
}

void Parktronic::setCenterRear(byte val)
{
  this->data.byte3 = (this->data.byte3 & 0b11100011) + ((val & 0b111) << 2) ;
}

void Parktronic::setRightRear(byte val)
{
  this->data.byte4 = (this->data.byte4 & 0b00011111) + ((val & 0b111) << 5) ;
}

void Parktronic::setShowWindow(bool val)
{
  this->data.byte5 = ((this->data.byte5 & 0b11111101) + ((val & 0b1) << 1));
}

void Parktronic::getMsgBuf(byte *ptr)
{
  ptr[0]= this->data.byte0;
  ptr[1]= this->data.byte1;
  ptr[2]= this->data.byte2;
  ptr[3]= this->data.byte3;
  ptr[4]= this->data.byte4;
  ptr[5]= this->data.byte5;
  ptr[6]= this->data.byte6;
  //ptr[7]= this->data.byte7;
}


MCP_CAN CAN0(10);     // Set CS to pin 10
int  switchState_A=0;
int prevswitchState_A= LOW;
int  switchState_B=0;
int prevswitchState_B= LOW;
//PDC parktronic = {0b111,0b111,0b111,0b111,0b111,0b111,0};

Parktronic parktronic;

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
  
  if (parktronic.getShowWindow()){ 
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
    
   if (parktronic.getShowWindow()){
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
  switchState_A = digitalRead(3);
  if (switchState_A == LOW && prevswitchState_A == HIGH){
      Serial.println("Pressed_A");
      //Serial.println(parktronic.getShowWindow());
      parktronic.setShowWindow ( !((bool)parktronic.getShowWindow()));
      //Serial.println(parktronic.getShowWindow());
      
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
    parktronic.setCenterRear(abs(7));
    parktronic.setLeftRear(abs(7));
    parktronic.setRightRear(abs(7));
    parktronic.setCenterFront(abs(i));
    parktronic.setLeftFront(abs(j));
    parktronic.setRightFront(abs(k));
 }else{
    parktronic.setCenterRear(abs(i));
    parktronic.setLeftRear(abs(j));
    parktronic.setRightRear(abs(k));
    parktronic.setCenterFront(abs(7));
    parktronic.setLeftFront(abs(7));
    parktronic.setRightFront(abs(7));
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
