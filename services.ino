void Interface::services () {
  sleepen = false;
  //tft.drawString("Services",120,40,2);
  if(substate == -1){substate = 0;}
  if(substate == 0){
    if (WiFi.status() == WL_CONNECTED){tft.drawString("Wifi Connected",120,60,2);tft.drawString((WiFi.SSID()+" "+WiFi.localIP().toString()).c_str(),120,80,2);}
    if (WiFi.status() == WL_NO_SHIELD){tft.drawString("Wifi No Shield",120,60,2);}
    if (WiFi.status() == WL_IDLE_STATUS){tft.drawString("Wifi Idle",120,60,2);}
    if (WiFi.status() == WL_CONNECT_FAILED){tft.drawString("Wifi Connect Failed",120,60,2);}
    if (WiFi.status() == WL_NO_SSID_AVAIL){tft.drawString("Wifi No SSID",120,60,2);}
    if (WiFi.status() == WL_SCAN_COMPLETED){tft.drawString("Wifi Scan Completed",120,60,2);}
    if (WiFi.status() == WL_CONNECTION_LOST){tft.drawString("Wifi Connect Lost",120,60,2);}
    if (WiFi.status() == WL_DISCONNECTED){tft.drawString("Wifi Disconnected",120,60,2);}
    if (hotspoten){tft.drawString("Hotspot ON",120,100,2);tft.drawString(("AP: "+WiFi.softAPIP().toString()).c_str(),120,120,2);}else{tft.drawString("Hotspot OFF",120,100,2);}
    if (serveren){tft.drawString("SD Webserver ON use www.IP/edit",120,140,2);}else{tft.drawString("SD Webserver OFF",120,140,2);}
  }
  if(substate == 1){
    WifiToggleUI();
  }
  if(substate == 2){
    WifiTimeSyncUI();
  }
  if(substate == 3){
    SDWebServerUI();
  }
}
void Interface::WifiToggleUI(){
  if (wifien == false){
    drawPNG(40,40,"/data/wifi_dis.png");
    tft.drawArc(120, 120, 115, 110, 0, 360, TFT_YELLOW, TFT_BLACK);
    tft.drawString("Wifi OFF",120,40,2);
    if (centTouch == 2){
      WifiToggle();
      tft.fillCircle(120,120,118,TFT_BLACK);
      centTouch = 0;
    }
  }
  else if (wifien == true){
    drawPNG(40,40,"/data/wifi_enb.png");
    if (WiFi.status() == WL_CONNECTED){tft.drawArc(120, 120, 115, 110, 0, 360, TFT_GREEN, TFT_BLACK);tft.drawString(WiFi.SSID().c_str(),120,40,2);tft.drawString(WiFi.localIP().toString().c_str(),120,200,2);}
    else{tft.drawArc(120, 120, 115, 110, 0, 360, TFT_BLUE, TFT_BLACK);tft.drawString("Connecting",120,40,2);}
    if (centTouch == 2){
      tft.fillCircle(120,120,118,TFT_BLACK);
      WifiToggle();
      centTouch = 0;
    }
  }
}
void Interface::WifiTimeSyncUI(){
  drawPNG(40,40,"/data/time_syn.png");
  tft.drawString("Wifi Time Sync",120,40,2);
  if (centTouch == 2){
    tft.drawArc(120, 120, 115, 110, 0, 360, TFT_BLUE, TFT_BLACK);
    int timeOut = millis() + 7200;
    while ( WiFi.status() != WL_CONNECTED && timeOut>millis() ){delay ( 10 );battery();tft.drawArc(120, 120, 115, 110, (timeOut-millis())/20, 360, TFT_BLACK, TFT_BLACK);}
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
        tft.drawString((String(String((timeStruct.hours-timeStruct.hours%10)/10,DEC)+String(timeStruct.hours%10,DEC))+":"+String(String((timeStruct.minutes-timeStruct.minutes%10)/10,DEC)+String(timeStruct.minutes%10,DEC))+":"+String(String((timeStruct.seconds-timeStruct.seconds%10)/10,DEC)+String(timeStruct.seconds%10,DEC))).c_str(),120,216,2);
        tft.drawArc(120, 120, 115, 110, 0, 360, TFT_GREEN, TFT_BLACK);
      }
      else{tft.drawString("Network failed",120,200,2);tft.drawArc(120, 120, 115, 110, 0, 360, TFT_RED, TFT_BLACK);}
    }
    else{tft.drawString("Wifi failed",120,200,2);tft.drawArc(120, 120, 115, 110, 0, 360, TFT_RED, TFT_BLACK);}
    centTouch = 0;
  }
}
void Interface::SDWebServerUI(){
  tft.drawString("Wifi File Server",120,40,2);
  if (serveren == false){
    drawPNG(40,40,"/data/serv_dis.png");
    tft.drawArc(120, 120, 115, 110, 0, 360, TFT_YELLOW, TFT_BLACK);
    if (centTouch == 2){
      if(!wifien){
        if(!hotspoten){HotspotToggle();}
      }
      serveren = true;
      server.begin();
      centTouch = 0;
      tft.fillCircle(120,120,118,TFT_BLACK);
    }
  }
  else if (serveren == true){
    drawPNG(40,40,"/data/serv_enb.png");
    tft.drawArc(120, 120, 115, 110, 0, 360, TFT_BLUE, TFT_BLACK);
    if (wifien && hotspoten){tft.drawString((WiFi.SSID()).c_str(),120,193,1);tft.drawString((WiFi.localIP().toString()).c_str(),120,203,1);tft.drawString(("AP "+WiFi.softAPIP().toString()).c_str(),120,213,1);}
    else if (hotspoten){tft.drawString(("AP "+WiFi.softAPIP().toString()).c_str(),120,200,2);}
    else if (wifien){tft.drawString((WiFi.SSID()).c_str(),120,196,2);tft.drawString((WiFi.localIP().toString()).c_str(),120,210,1);}
    if (centTouch == 2){
      serveren = false;
      server.stop();
      if(hotspoten){HotspotToggle();}
      centTouch = 0;
      tft.fillCircle(120,120,118,TFT_BLACK);
    }
  }
}
