#include "Arduino.h"
#include "PSA_CAN.h"

msgId_0E1::msgId_0E1()
{
  this->data = {0b00000000,0b00000000,0b00101010,0b00000000,0b00000000,0b00000000,0b11111111};
}


byte msgId_0E1::getFrontLeftValue()
{
 return(byte)max((3 - (getFrontLeftBits())),0);
}

byte msgId_0E1::getFrontCenterValue()
{
  return(byte) 4 - ceil((getFrontCenterBits())/2.0);
}

byte msgId_0E1::getFrontRightValue()
{
  return(byte)max((3 - (getFrontRightBits())),0);
}

byte msgId_0E1::getRearLeftValue()
{
  return(byte)  max((3 - (getRearLeftBits())),0);
}

byte msgId_0E1::getRearCenterValue()
{
  return(byte) 4 - ceil((getRearCenterBits())/2.0);
}

byte msgId_0E1::getRearRightValue()
{
  return(byte)max((3 - (getRearRightBits())),0);
}

byte msgId_0E1::getFrontLeftBits()
{
  return (byte)((this->data.byte4 >> 2) & 0b111);
}

byte msgId_0E1::getFrontCenterBits()
{
  return (byte)((this->data.byte5 >> 5) & 0b111);
}

byte msgId_0E1::getFrontRightBits()
{
  return (byte)((this->data.byte5 >> 2) & 0b111);
}

byte msgId_0E1::getRearLeftBits()
{
  return (byte)((this->data.byte3 >> 5) & 0b111);
}

byte msgId_0E1::getRearCenterBits()
{
  return (byte)((this->data.byte3 >> 2) & 0b111);
}

byte msgId_0E1::getRearRightBits()
{
  return (byte)((this->data.byte4 >> 5) & 0b111);
}

bool msgId_0E1::getShowWindowValue()
{ 
  return (bool)((this->data.byte5 & 0b00000010) >> 1);
}

void msgId_0E1::setFrontLeftBits(byte val)
{
  this->data.byte4 = (this->data.byte4 & 0b11100011) + ((val & 0b111) << 2) ;
}

void msgId_0E1::setFrontCenterBits(byte val)
{
  this->data.byte5 = (this->data.byte5 & 0b00011111) + ((val & 0b111) << 5) ;
}

void msgId_0E1::setFrontRightBits(byte val)
{
  this->data.byte5 = (this->data.byte5 & 0b11100011) + ((val & 0b111) << 2) ;
}

void msgId_0E1::setRearLeftBits(byte val)
{
  this->data.byte3 = (this->data.byte3 & 0b00011111) + ((val & 0b111) << 5) ;
}

void msgId_0E1::setRearCenterBits(byte val)
{
  this->data.byte3 = (this->data.byte3 & 0b11100011) + ((val & 0b111) << 2) ;
}

void msgId_0E1::setRearRightBits(byte val)
{
  this->data.byte4 = (this->data.byte4 & 0b00011111) + ((val & 0b111) << 5) ;
}

void msgId_0E1::setShowWindowValue(bool val)
{
  this->data.byte5 = ((this->data.byte5 & 0b11111101) + ((val & 0b1) << 1));
}

void msgId_0E1::getMsgBuf(byte ptr[])
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

void msgId_0E1::setMsgBuf(byte *pData)
{       
   //this->data = {*(pData+0),*(pData+1),*(pData+2),*(pData+3),*(pData+4),*(pData+5),*(pData+6)};
  this->MessageChanged = false;

  if (this->data.byte0 != *(pData+0)){
          this->data.byte0 = *(pData+0);
          this->MessageChanged = true;
  }

  if (this->data.byte1 != *(pData+1)){
          this->data.byte1 = *(pData+1);
          this->MessageChanged = true;
  }

  if (this->data.byte2 != *(pData+2)){
          this->data.byte2 = *(pData+2);
          this->MessageChanged = true;
  }

  if (this->data.byte3 != *(pData+3)){
          this->data.byte3 = *(pData+3);
          this->MessageChanged = true;
  }

  if (this->data.byte4 != *(pData+4)){
          this->data.byte4 = *(pData+4);
          this->MessageChanged = true;
  }

  if (this->data.byte5 != *(pData+5)){
          this->data.byte5 = *(pData+5);
          this->MessageChanged = true;
  }

  if (this->data.byte6 != *(pData+6)){
          this->data.byte6 = *(pData+6);
          this->MessageChanged = true;
  }
/*
  if (this->data.byte7 != *(pData+7);){
          this->data.byte7 = *(pData+7);
          this->MessageChanged = true;
  }

  */ 
  /*
  this->data.byte0 = *(pData+0);
  this->data.byte1 = *(pData+1);
  this->data.byte2 = *(pData+2);
  this->data.byte3 = *(pData+3);
  this->data.byte4 = *(pData+4);
  this->data.byte5 = *(pData+5);
  this->data.byte6 = *(pData+6);
  //this->data.byte7 = *(pData+7);;
  */
}



msgId_276::msgId_276()
{
  this->data = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};
}


int msgId_276::getHourValue()
{ 
  int hours = (this->data.byte3 & 0b00011111);
  if (!getShow24HValue() && (hours > 12)){
        hours = hours -12;
      }
  return (int) hours ;
} 

int msgId_276::getMinutesValue()
{
  return (int) (this->data.byte4   & 0b00111111) ;
} 

bool msgId_276::getShow24HValue()
{
 return (bool) ((this->data.byte0 & 0b10000000) >> 7);
}

void msgId_276::getMsgBuf(byte ptr[])
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

void msgId_276::setMsgBuf(byte *pData)
{       
   //this->data = {*(pData+0),*(pData+1),*(pData+2),*(pData+3),*(pData+4),*(pData+5),*(pData+6)};
  this->MessageChanged = false;

  if (this->data.byte0 != *(pData+0)){
          this->data.byte0 = *(pData+0);
          this->MessageChanged = true;
  }

  if (this->data.byte1 != *(pData+1)){
          this->data.byte1 = *(pData+1);
          this->MessageChanged = true;
  }

  if (this->data.byte2 != *(pData+2)){
          this->data.byte2 = *(pData+2);
          this->MessageChanged = true;
  }

  if (this->data.byte3 != *(pData+3)){
          this->data.byte3 = *(pData+3);
          this->MessageChanged = true;
  }

  if (this->data.byte4 != *(pData+4)){
          this->data.byte4 = *(pData+4);
          this->MessageChanged = true;
  }

  if (this->data.byte5 != *(pData+5)){
          this->data.byte5 = *(pData+5);
          this->MessageChanged = true;
  }

  if (this->data.byte6 != *(pData+6)){
          this->data.byte6 = *(pData+6);
          this->MessageChanged = true;
  }
/*
  if (this->data.byte7 != *(pData+7);){
          this->data.byte7 = *(pData+7);
          this->MessageChanged = true;
  }

  */ 

}



