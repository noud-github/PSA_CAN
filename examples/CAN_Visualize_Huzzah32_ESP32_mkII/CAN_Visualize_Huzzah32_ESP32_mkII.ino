/*********************************************************************
*********************************************************************/

#include <ESP32CAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adapted_SSD1306.h>
#include <driver/rtc_io.h>
#include <PSA_CAN.h>
#include "Display7215.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Display7215 Screen(&display);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


char msgString[128];        
CAN_device_t CAN_cfg;
msgId_0E1 Parktronic;
msgId_276 BSI_time;
msgId_0F6 Temperature;

unsigned long lastMessage = 0;

void setup()   {                
  
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
      //Clock
      BSI_time.setMsgBuf(rx_frame.data.u8);
      //Serial.println (BSI_time.getMessageChanged());
    }

    if (rx_frame.MsgID == 0x0F6){
      //Temprature
      Temperature.setMsgBuf(rx_frame.data.u8);
    }
    
    if (rx_frame.MsgID == 0x0E1){
      //parktronic
       Parktronic.setMsgBuf(rx_frame.data.u8);
       //Serial.println (parktronic.getMessageChanged());
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
    
    if (BSI_time.MessageChanged or Parktronic.MessageChanged or Temperature.MessageChanged ) {
      display.clearDisplay();
      Screen.DrawClock(BSI_time);
      Screen.DrawTemperature(Temperature);
      Screen.DrawPDC(Parktronic);
      display.display();
      BSI_time.MessageChanged = false;
      Parktronic.MessageChanged  = false;
      Temperature.MessageChanged = false;
      Serial.print(" refreshed ");
    }

    Serial.println();
    
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


