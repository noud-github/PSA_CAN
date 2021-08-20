#ifndef Parktronic_h
#define Parktronic_h

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

#endif
