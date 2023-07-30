void Interface::begin () {
  //ServerInit
  server.on("/list", HTTP_GET, printDirectory);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, []() {returnOK();}, handleFileUpload);
  server.onNotFound(handleNotFound);
  
  //RTC & Display & SD Init
  pinMode(D0, INPUT);
  pinMode(TOUCH_INT, INPUT_PULLUP);
  Wire.begin();
  
  if(SD.begin(D2)){hasSD = true;}
  WifiInit();
  
  rtc.begin();
  rtc.getTime(&timeStruct);
  tft.init();
  tft.setRotation(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  tft.fillCircle(120,120,118,TFT_BLACK);
  //Sleep & Update & Battery setup
  targetTime = millis();
  sleepTime = millis() + 8192;
  for (int a=0; a<20; a++){
    vBat[a]=analogRead(D0);
  }
}

void Interface::tick () {
  if (targetTime < millis()) {
    rtc.getTime(&timeStruct);
    targetTime = millis() + 100;
    if (state == 0){
      if (state != laststate){lh=-1;lm=-1;ls=-1;}
      watchFace();
    }
    if (state == 1){
      if (state != laststate){}
      services();
    }
    laststate=state;
    battery();
  }
  if (sleepTime < millis() && sleepen){
    UIsleep();
  }
  touchTick();
  if (serveren){server.handleClient();}
  if (wifiTime < millis() && wifien){
    if (WiFi.status() != WL_CONNECTED){
      if(wifiMulti.run(7200)!=WL_CONNECTED){wifien=false;}
    }
    wifiTime = millis() + 30000;
  }
}

void Interface::watchFace () {
  sleepen = true;
  if (substate != 0){substate == 0;}
  if (lh != timeStruct.hours){tft.drawString((char *)String(String((timeStruct.hours-timeStruct.hours%10)/10,DEC)+String(timeStruct.hours%10,DEC)).c_str(), 60, 120,8);lh = timeStruct.hours;}
  if (lm != timeStruct.minutes){tft.drawString((char *)String(String((timeStruct.minutes-timeStruct.minutes%10)/10,DEC)+String(timeStruct.minutes%10,DEC)).c_str(), 180, 120,8);lm = timeStruct.minutes;}
  if (ls != timeStruct.seconds){tft.drawString((char *)String(String((timeStruct.seconds-timeStruct.seconds%10)/10,DEC)+String(timeStruct.seconds%10,DEC)).c_str(), 180, 182, 6);ls = timeStruct.seconds;}
  if (centTouch == 2){centTouch = 0;}
}

void Interface::UIsleep (){
  tft.fillCircle(120,120,118,TFT_BLACK);
  //Wifi Shutdown
  esp_wifi_disconnect();
  esp_wifi_stop();
  esp_wifi_deinit();
  //Shutdown everything else
  tft.writecommand(0x10);
  Wire.end();
  SPI.end();
  //Wakeup on touch
  esp_deep_sleep_enable_gpio_wakeup(BIT(D1), ESP_GPIO_WAKEUP_GPIO_LOW);
  esp_deep_sleep_start();
}
