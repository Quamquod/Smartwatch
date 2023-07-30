#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>

#include <esp_wifi.h>
#include <WiFi.h>
//#include "WiFiMulti.h"
#include <vector>
#include <limits.h>
#include <string.h>
#include <esp32-hal.h>

typedef struct {
    char * ssid;
    char * passphrase;
} WifiAPlist_t;

class WifiMul
{
public:
    WifiMul();
    ~WifiMul();

    bool addAP(const char* ssid, const char *passphrase = NULL);

    uint8_t run(uint32_t connectTimeout=5000);

private:
    std::vector<WifiAPlist_t> APlist;
} wifiMulti;

//#include "Network.h"
//#include "Sys_Variables.h"
//#include "CSS.h"

//Round display pins
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define CHSC6X_I2C_ID 0x2e
#define CHSC6X_MAX_POINTS_NUM 1
#define CHSC6X_READ_POINT_LEN 5
#define TOUCH_INT D1
#define XIAO_BL D6
#define XIAO_DC D3
#define XIAO_CS D7

//RTC
#include <I2C_BM8563.h>
I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire);
I2C_BM8563_TimeTypeDef timeStruct;
I2C_BM8563_DateTypeDef dateStruct;

//WebServer
#include <WiFiClient.h>
#include <WebServer.h>
WebServer server(80);

//UI
#include <TFT_eSPI.h>
#include <SPI.h>
//#include "time_sync.h"
#include <PNGdec.h>
//#define USE_TFT_ESPI_LIBRARY
//#include "lv_xiao_round_screen.h"
//lv_coord_t touchX, touchY;

class Interface{
  private:
    int centTouch = 0;
    int lh = -1;
    int lm = -1;
    int ls = -1;
    int w = 240;
    int h = 240;
    int laststate = 0;
    int state = 0; // 0 (Default Watchface) (UP DOWN)
    int substate = 0; // 0 (Default window) (LEFT RIGHT)
    int vBat[20];
    int touchLast = 0;
    uint32_t targetTime;
    uint32_t sleepTime;
    uint32_t wifiTime;
    void watchFace();
    void battery();
    void services();
    void UIsleep();
    int touchZone();
    void drawPNG(int16_t x, int16_t y, const char *name);
    void touchTick();
    void WifiTimeSyncUI();
    void SDWebServerUI();
    void WifiInit();
    void WifiToggle();
    void WifiToggleUI();
    void HotspotToggle();
  public:
    bool wifien = false;
    bool sleepen = true;
    bool serveren = false;
    bool hotspoten = false;
    bool hasSD = false;
    TFT_eSPI tft = TFT_eSPI();  
    TFT_eSprite face = TFT_eSprite(&tft);
    void begin();
    void tick();
} UI;

void setup() {
  //Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println("Booting...");
  UI.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  UI.tick();
  delay(15);
}
