/*********************************************************************
*********************************************************************/

#include <ESP32CAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adapted_SSD1306.h>
#include <driver/rtc_io.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

char msgString[128];        
CAN_device_t CAN_cfg;

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

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
byte Display_PDC = 0;
byte Front_Center_Value = 0;
byte Front_Left_Value = 0;
byte Front_Right_Value = 0;
byte Rear_Center_Value = 0;
byte Rear_Left_Value = 0;
byte Rear_Right_Value = 0;
int hours = 0;
int minutes = 0;
int temperature = 30;
bool DataChanged = 1;
bool Show_24h = true;
byte Show_parktronic_value = 0;
unsigned long lastMessage = 0;

void setup()   {                
  //Wire.setClock(3400000);
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  rtc_gpio_deinit(GPIO_NUM_4); 
  CAN_cfg.speed=CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
  //start CAN Module
  ESP32Can.CANInit();
  Serial.println("CAN Visualize Demo By 80\"");
  lastMessage = millis();
}

void loop() {

  CAN_frame_t rx_frame;
    //receive next CAN frame from queue
  if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
    lastMessage = millis();
      //do stuff!
    if((rx_frame.MsgID & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rx_frame.MsgID & 0x1FFFFFFF), rx_frame.FIR.B.DLC);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX DLC: %1d  Data:", rx_frame.MsgID, rx_frame.FIR.B.DLC);
    
    if (rx_frame.MsgID == 0x276){
      //Show 24H
      Show_24h = bool ((rx_frame.data.u8[0] & 0b10000000) >> 7);
      //hours
      hours = (rx_frame.data.u8[3] & 0b00011111) ;
      if (!Show_24h && (hours > 12)){
        hours = hours -12;
      }
      //min
      minutes = (rx_frame.data.u8[4] & 0b00111111) ;
      DataChanged = 1;
    }
    if (rx_frame.MsgID == 0x0E1){
       //byte rx_Show_parktronic_value = (rx_frame.data.u8[5] >> 1)  & 0b1 ;
        byte rx_Show_parktronic_value = (rx_frame.data.u8[5] & 0b00000010) >> 1;
        if (Show_parktronic_value != rx_Show_parktronic_value){
          Show_parktronic_value = rx_Show_parktronic_value;
          DataChanged = 1;
       }
       if (Show_parktronic_value) { 
        byte rx_rear_left_value = max((3 - (rx_frame.data.u8[3] >> 5)),0);
        if (rx_rear_left_value != Rear_Left_Value){
          Rear_Left_Value = rx_rear_left_value;
          DataChanged = 1;
        }
        byte rx_rear_center_value = 4 - ceil(((rx_frame.data.u8[3] >> 2) & 0b111)/2.0);
        if (Rear_Center_Value != rx_rear_center_value) {
          Rear_Center_Value = rx_rear_center_value;
          DataChanged = 1;
        }
        byte rx_rear_right_value = max((3 - (rx_frame.data.u8[4] >> 5)),0);
        if (Rear_Right_Value != rx_rear_right_value){
          Rear_Right_Value = rx_rear_right_value;
          DataChanged = 1;
        }
        byte rx_front_left_value = max((3 - ceil(((rx_frame.data.u8[4]>> 2) & 0b111)/2.0)),0);
        if (Front_Left_Value != rx_front_left_value){
          Front_Left_Value = rx_front_left_value;
          DataChanged = 1;
        }
        byte rx_front_center_value = 4- ceil((rx_frame.data.u8[5]>> 5)/2.0);
        if (Front_Center_Value != rx_front_center_value){
          Front_Center_Value = rx_front_center_value;
          DataChanged = 1;
        }
        byte rx_front_right_value = max ((3 - ceil(((rx_frame.data.u8[5] >> 2) & 0b111)/2.0)),0);
        if (Front_Right_Value != rx_front_right_value){
          Front_Right_Value = rx_front_right_value;
          DataChanged = 1;
        }
       }
    }
    
    Serial.print(msgString);
    
    if((rx_frame.MsgID & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
      //display.print(msgString);
    } else {
      for(byte i = 0; i<rx_frame.FIR.B.DLC; i++){
        sprintf(msgString, " 0x%.2X", rx_frame.data.u8[i]);
        Serial.print(msgString);
        //display.print(msgString);
      }
    }
    if (DataChanged) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      if (hours<10){
        display.setCursor(1,0);
      }else{
        display.setCursor(0,0);
      }
      display.print(hours);
      display.print(":");
      if (minutes<10) display.print('0');
      display.print(minutes);
      display.setCursor(110,0);
      if (temperature < 0 | temperature >= 100){
        display.setCursor(110,0);
      }else{
        display.setCursor(111,0);
      }
      display.print(temperature);

    if (Show_parktronic_value == 1){
      SHOW_PDC_CAR(1);
      Visualize_PDC_Rear_Center (Rear_Center_Value);
      Visualize_PDC_Rear_Left (Rear_Left_Value);
      Visualize_PDC_Rear_Right (Rear_Right_Value);
      Visualize_PDC_Front_Center (Front_Center_Value);
      Visualize_PDC_Front_Left (Front_Left_Value);
      Visualize_PDC_Front_Right (Front_Right_Value);
      display.display();
    } else {
      SHOW_PDC_CAR(0);
      Visualize_PDC_Rear_Center (0);
      Visualize_PDC_Rear_Left (0);
      Visualize_PDC_Rear_Right (0);
      Visualize_PDC_Front_Center (0);
      Visualize_PDC_Front_Left (0);
      Visualize_PDC_Front_Right (0);
      display.display();  
    }
    
    
    }

    Serial.println();
    DataChanged = 0;
  }else{
    if ((millis() -lastMessage ) > 10000 ){                          // wait a couple seconds, then put the ESP32 to sleep
        Serial.println("going to sleep!");
        display.clearDisplay();
        display.display();
        delay(2000);
        CAN_stop();
        delay(5);
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, LOW);    // enable the external wakeup on GPIO 4 (which is tied to CAN Rx)
        esp_deep_sleep_start();    // ESP32 goes to sleep until it sees GPIO-4 (CAN Rx) change state as a "wake up" message comes in
      }
  }
   
}

void Visualize_PDC_Front_Right(int Value){
  //Serial.println(Value);
  SHOW_PDC_FRONT_RIGHT_1  (0);
  SHOW_PDC_FRONT_RIGHT_2  (0);
  SHOW_PDC_FRONT_RIGHT_3  (0);
  switch(Value){
    case 3:
      SHOW_PDC_FRONT_RIGHT_1  (1);
      
    case 2:
      SHOW_PDC_FRONT_RIGHT_2  (1);
     
    case 1:
      SHOW_PDC_FRONT_RIGHT_3  (1);
   }
}

void Visualize_PDC_Rear_Right(int Value){
  //Serial.println(Value);
  SHOW_PDC_REAR_RIGHT_1  (0);
  SHOW_PDC_REAR_RIGHT_2  (0);
  SHOW_PDC_REAR_RIGHT_3  (0);
  switch(Value){
    case 3:
      SHOW_PDC_REAR_RIGHT_1  (1);
      
    case 2:
      SHOW_PDC_REAR_RIGHT_2  (1);
     
    case 1:
      SHOW_PDC_REAR_RIGHT_3  (1);
   }
}

void Visualize_PDC_Front_Left(int Value){
  //Serial.println(Value);
  SHOW_PDC_FRONT_LEFT_1  (0);
  SHOW_PDC_FRONT_LEFT_2  (0);
  SHOW_PDC_FRONT_LEFT_3  (0);
  switch(Value){
    case 3:
      SHOW_PDC_FRONT_LEFT_1  (1);
      
    case 2:
      SHOW_PDC_FRONT_LEFT_2  (1);
     
    case 1:
      SHOW_PDC_FRONT_LEFT_3  (1);
   }
}

void Visualize_PDC_Rear_Left(int Value){
  //Serial.println(Value);
  SHOW_PDC_REAR_LEFT_1  (0);
  SHOW_PDC_REAR_LEFT_2  (0);
  SHOW_PDC_REAR_LEFT_3  (0);
  switch(Value){
    case 3:
      SHOW_PDC_REAR_LEFT_1  (1);
      
    case 2:
      SHOW_PDC_REAR_LEFT_2  (1);
     
    case 1:
      SHOW_PDC_REAR_LEFT_3  (1);
   }
}

void Visualize_PDC_Rear_Center(int Value){
  //Serial.println(Value);
  SHOW_PDC_REAR_CENTER_1  (0);
  SHOW_PDC_REAR_CENTER_2  (0);
  SHOW_PDC_REAR_CENTER_3  (0);
  SHOW_PDC_REAR_CENTER_4  (0);
  switch(Value){
    case 4:
      SHOW_PDC_REAR_CENTER_1  (1);
      
    case 3:
      SHOW_PDC_REAR_CENTER_2  (1);
     
    case 2:
      SHOW_PDC_REAR_CENTER_3  (1);
     
    case 1:
      SHOW_PDC_REAR_CENTER_4  (1);
    }
}

void Visualize_PDC_Front_Center(int Value){
  //Serial.println(Value);
  SHOW_PDC_FRONT_CENTER_1  (0);
  SHOW_PDC_FRONT_CENTER_2  (0);
  SHOW_PDC_FRONT_CENTER_3  (0);
  SHOW_PDC_FRONT_CENTER_4  (0);
  switch(Value){
    case 4:
      SHOW_PDC_FRONT_CENTER_1  (1);
      
    case 3:
      SHOW_PDC_FRONT_CENTER_2  (1);
     
    case 2:
      SHOW_PDC_FRONT_CENTER_3  (1);
     
    case 1:
      SHOW_PDC_FRONT_CENTER_4  (1);
    }
}

void SHOW_PDC_CAR(bool on){
 if (on)
    display.drawBitmap(XPOS + 17, 0,  car, 48, 32,  WHITE);
 else      
    display.drawBitmap(XPOS + 17, 0,  car, 48, 32,  BLACK);
}

void SHOW_PDC_REAR_CENTER_1(bool on){
 if (on)
    display.drawBitmap(XPOS +15, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS +15, 9,  PDC_MID, 8, 14,  BLACK);
}
void SHOW_PDC_REAR_CENTER_2(bool on){
 if (on)
    display.drawBitmap(XPOS +10, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS +10, 9,  PDC_MID, 8, 14,  BLACK);
}
void SHOW_PDC_REAR_CENTER_3(bool on){
 if (on)
    display.drawBitmap(XPOS +5, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS +5, 9,  PDC_MID, 8, 14,  BLACK);
}
void SHOW_PDC_REAR_CENTER_4(bool on){
 if (on)
    display.drawBitmap(XPOS, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS, 9,  PDC_MID, 8, 14,  BLACK);
}



void SHOW_PDC_REAR_RIGHT_1(bool on){
 if (on)
    display.drawBitmap(XPOS +15, 24,  PDC_REAR_RIGHT_1, 8, 6,  WHITE);
 else
    display.drawBitmap(XPOS +15, 24,  PDC_REAR_RIGHT_1, 8, 6,  BLACK);
}

void SHOW_PDC_REAR_RIGHT_2(bool on){
 if (on)
    display.drawBitmap(XPOS +10, 24,  PDC_REAR_RIGHT_2, 8, 7,  WHITE);
 else
    display.drawBitmap(XPOS +10, 24,  PDC_REAR_RIGHT_2, 8, 7,  BLACK);
}

void SHOW_PDC_REAR_RIGHT_3(bool on){
 if (on)
    display.drawBitmap(XPOS +5, 24,  PDC_REAR_RIGHT_3, 8, 8,  WHITE);
 else
    display.drawBitmap(XPOS +5, 24,  PDC_REAR_RIGHT_3, 8, 8,  BLACK);
}


void SHOW_PDC_REAR_LEFT_1(bool on){
 if (on)
    display.drawBitmap(XPOS +15, 2,  PDC_REAR_LEFT_1, 8, 6,  WHITE);
 else
    display.drawBitmap(XPOS +15, 2,  PDC_REAR_LEFT_1, 8, 6,  BLACK);
}

void SHOW_PDC_REAR_LEFT_2(bool on){
 if (on)
    display.drawBitmap(XPOS +10, 1,  PDC_REAR_LEFT_2, 8, 7,  WHITE);
 else
    display.drawBitmap(XPOS +10, 1,  PDC_REAR_LEFT_2, 8, 7,  BLACK);
}

void SHOW_PDC_REAR_LEFT_3(bool on){
 if (on)
    display.drawBitmap(XPOS +5, 0,  PDC_REAR_LEFT_3, 8, 8,  WHITE);
 else
    display.drawBitmap(XPOS +5, 0,  PDC_REAR_LEFT_3, 8, 8,  BLACK);
}


void SHOW_PDC_FRONT_CENTER_1(bool on){
 if (on)
    display.drawBitmap(XPOS + 58, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS + 58, 9,  PDC_MID, 8, 14,  BLACK);
}

void SHOW_PDC_FRONT_CENTER_2(bool on){
 if (on)
    display.drawBitmap(XPOS + 63, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS + 63, 9,  PDC_MID, 8, 14,  BLACK);
}

void SHOW_PDC_FRONT_CENTER_3(bool on){
 if (on)
    display.drawBitmap(XPOS + 68, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS + 68, 9,  PDC_MID, 8, 14,  BLACK);
}

void SHOW_PDC_FRONT_CENTER_4(bool on){
 if (on)
    display.drawBitmap(XPOS + 73, 9,  PDC_MID, 8, 14,  WHITE);
 else
    display.drawBitmap(XPOS + 73, 9,  PDC_MID, 8, 14,  BLACK);
}

void SHOW_PDC_FRONT_LEFT_1(bool on){
 if (on)
    display.drawBitmap(XPOS + 58, 2,  PDC_FRONT_LEFT_1, 8, 6,  WHITE);
 else
    display.drawBitmap(XPOS + 58, 2,  PDC_FRONT_LEFT_1, 8, 6,  BLACK); 
}

void SHOW_PDC_FRONT_LEFT_2(bool on){
 if (on)
    display.drawBitmap(XPOS + 63, 1,  PDC_FRONT_LEFT_2, 8, 7,  WHITE);
 else
    display.drawBitmap(XPOS + 63, 1,  PDC_FRONT_LEFT_2, 8, 7,  BLACK);
}

void SHOW_PDC_FRONT_LEFT_3(bool on){
 if (on)
    display.drawBitmap(XPOS + 68, 0,  PDC_FRONT_LEFT_3, 8, 8,  WHITE);
 else
    display.drawBitmap(XPOS + 68, 0,  PDC_FRONT_LEFT_3, 8, 8,  BLACK);
}

void SHOW_PDC_FRONT_RIGHT_1(bool on){
 if (on)
    display.drawBitmap(XPOS + 58, 24,  PDC_FRONT_RIGHT_1, 8, 6,  WHITE); 
 else
    display.drawBitmap(XPOS + 58, 24,  PDC_FRONT_RIGHT_1, 8, 6,  BLACK);
 }

void SHOW_PDC_FRONT_RIGHT_2(bool on){
  if (on)
    display.drawBitmap(XPOS + 63, 24,  PDC_FRONT_RIGHT_2, 8, 7,  WHITE);
  else
    display.drawBitmap(XPOS + 63, 24,  PDC_FRONT_RIGHT_2, 8, 7,  BLACK);
}

void SHOW_PDC_FRONT_RIGHT_3(bool on){
  if (on)
    display.drawBitmap(XPOS + 68, 24,  PDC_FRONT_RIGHT_3, 8, 8,  WHITE);
  else
    display.drawBitmap(XPOS + 68, 24,  PDC_FRONT_RIGHT_3, 8, 8,  BLACK);
}



