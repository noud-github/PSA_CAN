#ifndef PSA_CAN_h
#define PSA_CAN_h

#define MAX_CHAR_IN_MESSAGE 8

#include <RTClib.h>

typedef struct {
  byte byte0;
  byte byte1;
  byte byte2;
  byte byte3;
  byte byte4;
  byte byte5;
  byte byte6;
  byte byte7;
} CAN_DATA_STRUCT;

class msgId_0E1 {
  private:
    CAN_DATA_STRUCT data;
    

  public:
    msgId_0E1();
    bool MessageChanged = false;
    const int Period = 100;
    byte getFrontLeftBits();
    byte getFrontCenterBits();
    byte getFrontRightBits();
    byte getRearLeftBits();
    byte getRearCenterBits();
    byte getRearRightBits();

    byte getFrontLeftValue();
    byte getFrontCenterValue();
    byte getFrontRightValue();
    byte getRearLeftValue();
    byte getRearCenterValue();
    byte getRearRightValue();

    bool getShowWindowValue();
    

    void setFrontLeftBits(byte val);
    void setFrontCenterBits(byte val);
    void setFrontRightBits(byte val);
    void setRearLeftBits(byte val);
    void setRearCenterBits(byte val);
    void setRearRightBits(byte val);

    void setShowWindowValue(bool val);

    void getMsgBuf(byte *ptr);
    void setMsgBuf(byte *ptr);
};




class msgId_276 {
  private:
    CAN_DATA_STRUCT data;
    //bool MessageChanged = false;

  public:
    const int Period = 1000;
    bool MessageChanged = false;

    msgId_276();
    int getHourValue();
    int getMinutesValue();
    bool getShow24HValue();

    void setTime(DateTime val);
    void getMsgBuf(byte *ptr);
    void setMsgBuf(byte *ptr);
};

#endif

