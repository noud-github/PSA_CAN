#include <ESP32CAN.h>
#include <CAN_config.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "driver/rtc_io.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//CAN_device_t CAN_cfg;
CAN_device_t CAN_cfg = {
  .speed=CAN_SPEED_125KBPS,    // CAN Node baudrade
  .tx_pin_id = GPIO_NUM_5,    // CAN TX pin
  .rx_pin_id = GPIO_NUM_4,    // CAN RX pin
  .rx_queue=NULL,         // FreeRTOS queue for RX frames
};


//unsigned char rxBuf[8] = {0,0,0,0,0,0,0,0};
char msgString[128];                        // Array to store serial string
bool DataChanged = true;
int hours = 0;
int minutes = 0;
int temperature = 21;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
    Serial.begin(115200);
   
    rtc_gpio_deinit(GPIO_NUM_4);    // reset the rtc_GPIO wake up pin to be a normal digital GPIO-4 again  
    Serial.println("iotsharing.com CAN demo");
    //CAN_cfg.speed=CAN_SPEED_125KBPS;
    //CAN_cfg.tx_pin_id = GPIO_NUM_5;
    //CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //start CAN Module
    ESP32Can.CANInit();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    display.display();
    delay(2000);
    // Clear the buffer.
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("00:00");
    display.setCursor(110,0);
    display.print("-00");
    display.display();
}

void loop() {
  
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      //do stuff!
      if(rx_frame.FIR.B.FF==CAN_frame_std)
        //Serial.println(rx_frame.MsgID);
        if (rx_frame.MsgID == 0x276){
          //hours
          hours = (rx_frame.data.u8[3] & 0b00011111) ;
         //min
          minutes = (rx_frame.data.u8[4] & 0b00111111) ;
          DataChanged = true;
        }
        
        //Serial.println(DataChanged);
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

      
    }
    //Serial.println()
    display.display();
    DataChanged = false;
    }
}
