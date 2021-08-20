#ifndef PSA_CAN_h
#define PSA_CAN_h

#define MAX_CHAR_IN_MESSAGE 8

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
  //DATA_0E1 data;
  byte  m_nDta[MAX_CHAR_IN_MESSAGE] = {0b00000000,0b00000000,0b00101010,0b00000000,0b00000000,0b00000000,0b11111111};
  
public:
  Parktronic();
  byte getLeftFrontBits();
  byte getCenterFrontBits();
  byte getRightFrontBits();
  byte getLeftRearBits();
  byte getCenterRearBits();
  byte getRightRearBits();
  bool getShowWindowBit();
  void setLeftFrontBits(byte val);
  void setCenterFrontBits(byte val);
  void setRightFrontBits(byte val);
  void setLeftRearBits(byte val);
  void setCenterRearBits(byte val);
  void setRightRearBits(byte val);
  void setShowWindowBit(bool val);
  void getMsgBuf(byte *ptr);
  void setMsgBuf(byte *ptr);
};

#endif
