#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <esp_wifi.h>
#include <WiFi.h>
//#include "Network.h"
//#include "Sys_Variables.h"
//#include "CSS.h"

//RTC
#include <I2C_BM8563.h>
I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire);
I2C_BM8563_TimeTypeDef timeStruct;
I2C_BM8563_DateTypeDef dateStruct;

//UI
#include <TFT_eSPI.h>
#include <SPI.h>
#define USE_TFT_ESPI_LIBRARY
#include "lv_xiao_round_screen.h"
lv_coord_t touchX, touchY;

class Interface{
  private:
    int lh = -1;
    int lm = -1;
    int ls = -1;
    int w = 240;
    int h = 240;
    int laststate = 0;
    int state = 0; // 0 (Default Watchface) (UP DOWN)
    int vBat[20];
    int touchLast = 0;
    uint32_t targetTime;
    TFT_eSPI tft = TFT_eSPI();  
    TFT_eSprite face = TFT_eSprite(&tft);
    void watchFace();
    void battery();
    void services();
    int touchZone();
  public:
    void begin();
    void tick();
} UI;

void setup() {
  //Serial.begin(115200);
  //Serial.println("Booting...");
  UI.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  UI.tick();
  delay(15);
}
