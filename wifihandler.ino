//Wifi
#include <Dictionary.h>
Dictionary *dict = new Dictionary();
const char * soft_ap_ssid = "Smartwatch_AP";
const char * soft_ap_password = "********";

void Interface::WifiTimeSync(){
  drawPNG(40,40,"/data/time_syn.png");
  if (centTouch == 2){
    WiFi.begin("dechtarka", "Dobak1938");
    tft.drawArc(120, 120, 115, 110, 0, 360, TFT_BLUE, TFT_BLACK);
    int timeOut = millis() + 9000;
    while ( WiFi.status() != WL_CONNECTED && timeOut>millis() ){delay ( 10 );battery();tft.drawArc(120, 120, 115, 110, (timeOut-millis())/25, 360, TFT_BLACK, TFT_BLACK);}
    if(WiFi.status()==WL_CONNECTED){
      configTime(3600,3600, "time.cloudflare.com");
      struct tm timeInfo;
      if (getLocalTime(&timeInfo)) {
        timeStruct.hours   = timeInfo.tm_hour;
        timeStruct.minutes = timeInfo.tm_min;
        timeStruct.seconds = timeInfo.tm_sec;
        rtc.setTime(&timeStruct);
        dateStruct.weekDay = timeInfo.tm_wday;
        dateStruct.month   = timeInfo.tm_mon + 1;
        dateStruct.date    = timeInfo.tm_mday;
        dateStruct.year    = timeInfo.tm_year + 1900;
        rtc.setDate(&dateStruct);
        tft.drawString("Time Updated",120,200,2);
        tft.drawArc(120, 120, 115, 110, 0, 360, TFT_GREEN, TFT_BLACK);
      }
      else{tft.drawString("Network failed",120,200,2);tft.drawArc(120, 120, 115, 110, 0, 360, TFT_RED, TFT_BLACK);}
    }
    else{tft.drawString("Wifi failed",120,200,2);tft.drawArc(120, 120, 115, 110, 0, 360, TFT_RED, TFT_BLACK);}
    centTouch = 0;
  }
}
