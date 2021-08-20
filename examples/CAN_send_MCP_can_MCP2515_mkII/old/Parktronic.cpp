#include "Arduino.h"
#include "Parktronic.h"

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

