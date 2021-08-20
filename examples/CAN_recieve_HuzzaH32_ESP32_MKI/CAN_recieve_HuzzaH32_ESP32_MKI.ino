#include <ESP32CAN.h>
#include <CAN_config.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adapted_SSD1306.h>
#include <driver/rtc_io.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

CAN_device_t CAN_cfg;

unsigned char rxBuf[8];
char msgString[128]; // Array to store serial string
unsigned long lastMessage = 0;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
    Serial.begin(115200);
    rtc_gpio_deinit(GPIO_NUM_4);    
    Serial.println("iotsharing.com CAN demo");
    CAN_cfg.speed=CAN_SPEED_125KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //start CAN Module
    ESP32Can.CANInit();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    display.display();
    delay(2000);
    // Clear the buffer.
    display.clearDisplay();
    display.display();
    lastMessage = millis();
}

void loop() {
    
    
   
  
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      lastMessage = millis();
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);


      //do stuff!
      if(rx_frame.FIR.B.FF==CAN_frame_std)
        sprintf(msgString, "Standard ID: 0x%.3lX DLC: %1d  Data:", rx_frame.MsgID, rx_frame.FIR.B.DLC);
        //sprintf(msgString,"New standard frame");
      else
        sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rx_frame.MsgID & 0x1FFFFFFF), rx_frame.FIR.B.DLC);
        //sprintf(msgString,"New extended frame");
      
      Serial.print(msgString);
      display.print(msgString);

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        //sprintf(msgString," from 0x%.3lX, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        //Serial.print(msgString);
        //display.print(msgString);
        for(int i = 0; i < rx_frame.FIR.B.DLC; i++){
          sprintf(msgString, " 0x%.2X", rx_frame.data.u8[i]);
          Serial.print(msgString);
          display.print(msgString);
        }
       
        Serial.println();
        display.display();
      }
      
    } else {
      
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
