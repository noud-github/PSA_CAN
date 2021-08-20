#ifndef Display7215_h
#define Display7215_h

#include <PSA_CAN.h>
#include <Adapted_SSD1306.h>
#define XPOS 25

static const unsigned char PROGMEM car []  ={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3E, 0x02, 0x03, 0xF0, 0x00, 0x00, 0x3E, 0x02, 0x03, 0xF0, 0x00, 0x00, 0xFF,
0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x03, 0x08, 0x20, 0xC0, 0x00, 0xC0,
0x03, 0x08, 0x20, 0xC0, 0x00, 0xC0, 0x03, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x03, 0x00, 0x1F, 0xF0,
0x00, 0x20, 0x03, 0x00, 0x00, 0x30, 0x00, 0x20, 0x03, 0x30, 0x00, 0x3C, 0x00, 0x20, 0x03, 0x30,
0x00, 0x3C, 0x00, 0x20, 0x03, 0x30, 0x00, 0x3C, 0x00, 0x20, 0x03, 0x30, 0x00, 0x3C, 0x00, 0x20,
0x03, 0x30, 0x00, 0x3C, 0x00, 0x20, 0x03, 0x30, 0x00, 0x3C, 0x00, 0x20, 0x03, 0x30, 0x00, 0x3C,
0x00, 0x20, 0x03, 0x30, 0x00, 0x3C, 0x00, 0x20, 0x03, 0x00, 0x00, 0x30, 0x00, 0x20, 0x03, 0x00,
0x1F, 0xF0, 0x00, 0x20, 0x03, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x03, 0x08, 0x20, 0xC0, 0x00, 0xC0,
0x03, 0x08, 0x20, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0xFF, 0x00, 0x00, 0x3E, 0x02, 0x03, 0xF0, 0x00, 0x00, 0x3E, 0x02, 0x03, 0xF0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM PDC_FRONT_RIGHT_1 [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00001110
};

static const unsigned char PROGMEM PDC_FRONT_RIGHT_2 [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00001110
};

static const unsigned char PROGMEM PDC_FRONT_RIGHT_3 [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00001110
};

static const unsigned char PROGMEM PDC_REAR_RIGHT_1 [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011100
};

static const unsigned char PROGMEM PDC_REAR_RIGHT_2 [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011100
};

static const unsigned char PROGMEM PDC_REAR_RIGHT_3 [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011100
};

static const unsigned char PROGMEM PDC_REAR_LEFT_1 [] = {
  B00011100,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110
};

static const unsigned char PROGMEM PDC_REAR_LEFT_2 [] = {
  B00011100,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110
};

static const unsigned char PROGMEM PDC_REAR_LEFT_3 [] = {
  B00011100,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110
};

static const unsigned char PROGMEM PDC_FRONT_LEFT_1 [] = {
  B00001110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110
};

static const unsigned char PROGMEM PDC_FRONT_LEFT_2 [] = {
  B00001110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110
};

static const unsigned char PROGMEM PDC_FRONT_LEFT_3 [] = {
  B00001110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110
};

static const unsigned char PROGMEM PDC_MID [] = {
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110,
  B00011110 
};



class Display7215
{
  public:
    Display7215(Adafruit_SSD1306* display);
    void DrawTemperature(msgId_0F6& temperature);
    void DrawPDC(msgId_0E1& parktronic);
    void DrawClock(msgId_276& BSI_time);
  private:
    Adafruit_SSD1306* _display;
    void Visualize_PDC_Front_Right(int Value);
    void Visualize_PDC_Rear_Right(int Value);
    void Visualize_PDC_Front_Left(int Value);
    void Visualize_PDC_Rear_Left(int Value);
    void Visualize_PDC_Rear_Center(int Value);
    void Visualize_PDC_Front_Center(int Value);
    void SHOW_PDC_CAR(uint16_t color);
    void SHOW_PDC_REAR_CENTER_1(uint16_t color);
    void SHOW_PDC_REAR_CENTER_2(uint16_t color);
    void SHOW_PDC_REAR_CENTER_3(uint16_t color);
    void SHOW_PDC_REAR_CENTER_4(uint16_t color);
    void SHOW_PDC_REAR_RIGHT_1(uint16_t color);
    void SHOW_PDC_REAR_RIGHT_2(uint16_t color);
    void SHOW_PDC_REAR_RIGHT_3(uint16_t color);
    void SHOW_PDC_REAR_LEFT_1(uint16_t color);
    void SHOW_PDC_REAR_LEFT_2(uint16_t color);
    void SHOW_PDC_REAR_LEFT_3(uint16_t color);
    void SHOW_PDC_FRONT_CENTER_1(uint16_t color);
    void SHOW_PDC_FRONT_CENTER_2(uint16_t color);
    void SHOW_PDC_FRONT_CENTER_3(uint16_t color);
    void SHOW_PDC_FRONT_CENTER_4(uint16_t color);
    void SHOW_PDC_FRONT_LEFT_1(uint16_t color);
    void SHOW_PDC_FRONT_LEFT_2(uint16_t color);
    void SHOW_PDC_FRONT_LEFT_3(uint16_t color);
    void SHOW_PDC_FRONT_RIGHT_1(uint16_t color);
    void SHOW_PDC_FRONT_RIGHT_2(uint16_t color);
    void SHOW_PDC_FRONT_RIGHT_3(uint16_t color);    

    
};

#endif
