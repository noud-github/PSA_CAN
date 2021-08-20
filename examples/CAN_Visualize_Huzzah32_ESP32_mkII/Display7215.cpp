
#include "Display7215.h"
#include <PSA_CAN.h>

#include <Adapted_SSD1306.h>

Display7215::Display7215(Adafruit_SSD1306* display){
  this->_display = display;
}

void Display7215::DrawClock(msgId_276& BSI_time){
  this->_display->setTextSize(1);
  this->_display->setTextColor(WHITE);
      if (BSI_time.getHourValue()<10){
        this->_display->setCursor(1,0);
      }else{
        this->_display->setCursor(0,0);
      }
      this->_display->print(BSI_time.getHourValue());
      this->_display->print(":");
      if (BSI_time.getMinutesValue()<10) this->_display->print('0');
      this->_display->print(BSI_time.getMinutesValue());
}

void Display7215::DrawPDC(msgId_0E1& parktronic){
  if (parktronic.getShowWindowValue()){
      this->SHOW_PDC_CAR(WHITE);
      this->Visualize_PDC_Rear_Center (parktronic.getRearCenterValue());
      this->Visualize_PDC_Rear_Left (parktronic.getRearLeftValue());
      this->Visualize_PDC_Rear_Right (parktronic.getRearRightValue());
      this->Visualize_PDC_Front_Center (parktronic.getFrontCenterValue());
      this->Visualize_PDC_Front_Left (parktronic.getFrontLeftValue());
      this->Visualize_PDC_Front_Right (parktronic.getFrontRightValue());
      
    } else {
      this->SHOW_PDC_CAR(BLACK);
      this->Visualize_PDC_Rear_Center (BLACK);
      this->Visualize_PDC_Rear_Left (BLACK);
      this->Visualize_PDC_Rear_Right (BLACK);
      this->Visualize_PDC_Front_Center (BLACK);
      this->Visualize_PDC_Front_Left (BLACK);
      this->Visualize_PDC_Front_Right (BLACK);
      
    }
}

void Display7215::DrawTemperature(msgId_0F6& temperature){
      this->_display->setCursor(110,0);
      if (temperature.getTemperatureValue() < 0 | temperature.getTemperatureValue() >= 100){
        this->_display->setCursor(110,0);
      }else{
        this->_display->setCursor(111,0);
      }
      this->_display->print(temperature.getTemperatureValue());
}



void Display7215::Visualize_PDC_Front_Right(int Value){
  //Serial.println(Value);
  //this->SHOW_PDC_FRONT_RIGHT_1  (BLACK);
  //this->SHOW_PDC_FRONT_RIGHT_2  (BLACK);
  //this->SHOW_PDC_FRONT_RIGHT_3  (BLACK);
  switch(Value){
    case 3:
      this->SHOW_PDC_FRONT_RIGHT_1  (WHITE);
      
    case 2:
      this->SHOW_PDC_FRONT_RIGHT_2  (WHITE);
     
    case 1:
      this->SHOW_PDC_FRONT_RIGHT_3  (WHITE);
   }
}

void Display7215::Visualize_PDC_Rear_Right(int Value){
  //Serial.println(Value);
  //this->SHOW_PDC_REAR_RIGHT_1  (BLACK);
  //this->SHOW_PDC_REAR_RIGHT_2  (BLACK);
  //this->SHOW_PDC_REAR_RIGHT_3  (BLACK);
  switch(Value){
    case 3:
      this->SHOW_PDC_REAR_RIGHT_1  (WHITE);
      
    case 2:
      this->SHOW_PDC_REAR_RIGHT_2  (WHITE);
     
    case 1:
      this->SHOW_PDC_REAR_RIGHT_3  (WHITE);
   }
}

void Display7215::Visualize_PDC_Front_Left(int Value){
  //Serial.println(Value);
  //this->SHOW_PDC_FRONT_LEFT_1  (BLACK);
  //this->SHOW_PDC_FRONT_LEFT_2  (BLACK);
  //this->SHOW_PDC_FRONT_LEFT_3  (BLACK);
  switch(Value){
    case 3:
      this->SHOW_PDC_FRONT_LEFT_1  (WHITE);
      
    case 2:
      this->SHOW_PDC_FRONT_LEFT_2  (WHITE);
     
    case 1:
      this->SHOW_PDC_FRONT_LEFT_3  (WHITE);
   }
}

void Display7215::Visualize_PDC_Rear_Left(int Value){
  //Serial.println(Value);
  //this->SHOW_PDC_REAR_LEFT_1  (BLACK);
  //this->SHOW_PDC_REAR_LEFT_2  (BLACK);
  //this->SHOW_PDC_REAR_LEFT_3  (BLACK);
  switch(Value){
    case 3:
      this->SHOW_PDC_REAR_LEFT_1  (WHITE);
      
    case 2:
      this->SHOW_PDC_REAR_LEFT_2  (WHITE);
     
    case 1:
      this->SHOW_PDC_REAR_LEFT_3  (WHITE);
   }
}

void Display7215::Visualize_PDC_Rear_Center(int Value){
  //Serial.println(Value);
  //this->SHOW_PDC_REAR_CENTER_1  (BLACK);
  //this->SHOW_PDC_REAR_CENTER_2  (BLACK);
  //this->SHOW_PDC_REAR_CENTER_3  (BLACK);
  //this->SHOW_PDC_REAR_CENTER_4  (BLACK);
  switch(Value){
    case 4:
      this->SHOW_PDC_REAR_CENTER_1  (WHITE);
      
    case 3:
      this->SHOW_PDC_REAR_CENTER_2  (WHITE);
     
    case 2:
      this->SHOW_PDC_REAR_CENTER_3  (WHITE);
     
    case 1:
      this->SHOW_PDC_REAR_CENTER_4  (WHITE);
    }
}

void Display7215::Visualize_PDC_Front_Center(int Value){
  //Serial.println(Value);
  //this->SHOW_PDC_FRONT_CENTER_1  (BLACK);
  //this->SHOW_PDC_FRONT_CENTER_2  (BLACK);
  //this->SHOW_PDC_FRONT_CENTER_3  (BLACK);
  //this->SHOW_PDC_FRONT_CENTER_4  (BLACK);
  switch(Value){
    case 4:
      this->SHOW_PDC_FRONT_CENTER_1  (WHITE);
      
    case 3:
      this->SHOW_PDC_FRONT_CENTER_2  (WHITE);
     
    case 2:
      this->SHOW_PDC_FRONT_CENTER_3  (WHITE);
     
    case 1:
      this->SHOW_PDC_FRONT_CENTER_4  (WHITE);
    }
}

void Display7215::SHOW_PDC_CAR(uint16_t color){
    this->_display->drawBitmap(XPOS + 17, 0,  car, 48, 32,  color);
}

void Display7215::SHOW_PDC_REAR_CENTER_1(uint16_t color){
    this->_display->drawBitmap(XPOS +15, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_REAR_CENTER_2(uint16_t color){
    this->_display->drawBitmap(XPOS +10, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_REAR_CENTER_3(uint16_t color){
    this->_display->drawBitmap(XPOS +5, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_REAR_CENTER_4(uint16_t color){
    this->_display->drawBitmap(XPOS, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_REAR_RIGHT_1(uint16_t color){
    this->_display->drawBitmap(XPOS +15, 24,  PDC_REAR_RIGHT_1, 8, 6,  color);
}

void Display7215::SHOW_PDC_REAR_RIGHT_2(uint16_t color){
    this->_display->drawBitmap(XPOS +10, 24,  PDC_REAR_RIGHT_2, 8, 7,  color);
}

void Display7215::SHOW_PDC_REAR_RIGHT_3(uint16_t color){
    this->_display->drawBitmap(XPOS +5, 24,  PDC_REAR_RIGHT_3, 8, 8,  color);
}

void Display7215::SHOW_PDC_REAR_LEFT_1(uint16_t color){
    this->_display->drawBitmap(XPOS +15, 2,  PDC_REAR_LEFT_1, 8, 6,  color);
}

void Display7215::SHOW_PDC_REAR_LEFT_2(uint16_t color){
    this->_display->drawBitmap(XPOS +10, 1,  PDC_REAR_LEFT_2, 8, 7,  color);
}

void Display7215::SHOW_PDC_REAR_LEFT_3(uint16_t color){
    this->_display->drawBitmap(XPOS +5, 0,  PDC_REAR_LEFT_3, 8, 8,  color);
}

void Display7215::SHOW_PDC_FRONT_CENTER_1(uint16_t color){
    this->_display->drawBitmap(XPOS + 58, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_FRONT_CENTER_2(uint16_t color){
    this->_display->drawBitmap(XPOS + 63, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_FRONT_CENTER_3(uint16_t color){
   this->_display->drawBitmap(XPOS + 68, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_FRONT_CENTER_4(uint16_t color){
    this->_display->drawBitmap(XPOS + 73, 9,  PDC_MID, 8, 14,  color);
}

void Display7215::SHOW_PDC_FRONT_LEFT_1(uint16_t color){
    this->_display->drawBitmap(XPOS + 58, 2,  PDC_FRONT_LEFT_1, 8, 6,  color);
} 

void Display7215::SHOW_PDC_FRONT_LEFT_2(uint16_t color){
    this->_display->drawBitmap(XPOS + 63, 1,  PDC_FRONT_LEFT_2, 8, 7,  color);
}

void Display7215::SHOW_PDC_FRONT_LEFT_3(uint16_t color){
    this->_display->drawBitmap(XPOS + 68, 0,  PDC_FRONT_LEFT_3, 8, 8,  color);
}

void Display7215::SHOW_PDC_FRONT_RIGHT_1(uint16_t color){
    this->_display->drawBitmap(XPOS + 58, 24,  PDC_FRONT_RIGHT_1, 8, 6,  color); 
}

void Display7215::SHOW_PDC_FRONT_RIGHT_2(uint16_t color){
    this->_display->drawBitmap(XPOS + 63, 24,  PDC_FRONT_RIGHT_2, 8, 7,  color);
}

void Display7215::SHOW_PDC_FRONT_RIGHT_3(uint16_t color){
  this->_display->drawBitmap(XPOS + 68, 24,  PDC_FRONT_RIGHT_3, 8, 8,  color);
}
