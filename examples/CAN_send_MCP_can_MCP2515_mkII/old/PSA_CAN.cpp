#include "Arduino.h"
#include "PSA_CAN.h"

Parktronic::Parktronic()
{
  //this->data = {0b00000000,0b00000000,0b00101010,0b00000000,0b00000000,0b00000000,0b11111111};
}

byte Parktronic::getLeftFrontBits()
{
  return (byte)((this->m_nDta[4] >> 2) & 0b111);
  //return (byte)((this->data.byte4 >> 2) & 0b111);
}

byte Parktronic::getCenterFrontBits()
{
  return (byte)((this->m_nDta[5] >> 5) & 0b111);
  //return (byte)((this->data.byte5 >> 5) & 0b111);
}

byte Parktronic::getRightFrontBits()
{
  return (byte)((this->m_nDta[5] >> 2) & 0b111);
  //return (byte)((this->data.byte5 >> 2) & 0b111);
}

byte Parktronic::getLeftRearBits()
{
  return (byte)((this->m_nDta[3] >> 3) & 0b111);
  //return (byte)((this->data.byte3 >> 5) & 0b111);
}

byte Parktronic::getCenterRearBits()
{
  return (byte)((this->m_nDta[3] >> 2) & 0b111);
  //return (byte)((this->data.byte3 >> 2) & 0b111);
}

byte Parktronic::getRightRearBits()
{
  return (byte)((this->m_nDta[4] >> 5) & 0b111);
  //return (byte)((this->data.byte4 >> 5) & 0b111);
}

bool Parktronic::getShowWindowBit()
{ 
  return (byte)((this->m_nDta[5] & 0b00000010) >> 1);
  //return (bool)((this->data.byte5 & 0b00000010) >> 1);
}

void Parktronic::setLeftFrontBits(byte val)
{
  this->m_nDta[4] = (this->m_nDta[4] & 0b11100011) + ((val & 0b111) << 2) ;
  //this->data.byte4 = (this->data.byte4 & 0b11100011) + ((val & 0b111) << 2) ;
}

void Parktronic::setCenterFrontBits(byte val)
{
  this->m_nDta[5] = (this->m_nDta[5] & 0b00011111) + ((val & 0b111) << 5) ;
  //this->data.byte5 = (this->data.byte5 & 0b00011111) + ((val & 0b111) << 5) ;
}

void Parktronic::setRightFrontBits(byte val)
{
  this->m_nDta[5] = (this->m_nDta[5] & 0b11100011) + ((val & 0b111) << 2) ;
  //this->data.byte5 = (this->data.byte5 & 0b11100011) + ((val & 0b111) << 2) ;
}

void Parktronic::setLeftRearBits(byte val)
{
  this->m_nDta[3] = (this->m_nDta[3]  & 0b00011111) + ((val & 0b111) << 5) ;
  //this->data.byte3 = (this->data.byte3 & 0b00011111) + ((val & 0b111) << 5) ;
}

void Parktronic::setCenterRearBits(byte val)
{
  this->m_nDta[3] = (this->m_nDta[3] & 0b11100011) + ((val & 0b111) << 2) ;
  //this->data.byte3 = (this->data.byte3 & 0b11100011) + ((val & 0b111) << 2) ;
}

void Parktronic::setRightRearBits(byte val)
{
  this->m_nDta[4] = (this->m_nDta[4]& 0b00011111) + ((val & 0b111) << 5) ;
  //this->data.byte4 = (this->data.byte4 & 0b00011111) + ((val & 0b111) << 5) ;
}

void Parktronic::setShowWindowBit(bool val)
{
  this->m_nDta[5] = ((this->m_nDta[5] & 0b11111101) + ((val & 0b1) << 1));
  //this->data.byte5 = ((this->data.byte5 & 0b11111101) + ((val & 0b1) << 1));
}

void Parktronic::getMsgBuf(byte ptr[])
{
  for(int i = 0; i< MAX_CHAR_IN_MESSAGE; i++)
        ptr[i] = m_nDta[i];
  //ptr[0]= this->data.byte0;
  //ptr[1]= this->data.byte1;
  //ptr[2]= this->data.byte2;
  //ptr[3]= this->data.byte3;
  //ptr[4]= this->data.byte4;
  //ptr[5]= this->data.byte5;
  //ptr[6]= this->data.byte6;
  //ptr[7]= this->data.byte7;
}

void Parktronic::setMsgBuf(byte *pData)
{
  for(int i = 0; i<MAX_CHAR_IN_MESSAGE; i++)
        m_nDta[i] = *(pData+i);
        
  //this->data.byte0 = *(pData+0);
  //this->data.byte1 = *(pData+1);
  //this->data.byte2 = *(pData+2);
  //this->data.byte3 = *(pData+3);
  //this->data.byte4 = *(pData+4);
  //this->data.byte5 = *(pData+5);
  //this->data.byte6 = *(pData+6);
  //this->data.byte7 = *(pData+7);;
}


/*

void Parktronic::setMsgBuf(byte *ptr)
{
  this->data.byte0 = ptr[0];
  this->data.byte1 = ptr[1];
  this->data.byte2 = ptr[2];
  this->data.byte3 = ptr[3];
  this->data.byte4 = ptr[4];
  this->data.byte5 = ptr[5];
  this->data.byte6 = ptr[6];
  //this->data.byte7 = prt[7];
}

*/
