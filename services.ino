void Interface::services () {
  sleepen = false;
  tft.drawString("Services",120,40,2);
  if(substate == -1){substate = 0;}
  if(substate == 0){
    if (WiFi.status() == WL_CONNECTED){tft.drawString("Wifi Connected",120,60,2);}
    if (WiFi.status() == WL_NO_SHIELD){tft.drawString("Wifi No Shield",120,60,2);}
    if (WiFi.status() == WL_IDLE_STATUS){tft.drawString("Wifi Idle",120,60,2);}
    if (WiFi.status() == WL_CONNECT_FAILED){tft.drawString("Wifi Connect Failed",120,60,2);}
    if (WiFi.status() == WL_NO_SSID_AVAIL){tft.drawString("Wifi No SSID",120,60,2);}
    if (WiFi.status() == WL_SCAN_COMPLETED){tft.drawString("Wifi Scan Completed",120,60,2);}
    if (WiFi.status() == WL_CONNECTION_LOST){tft.drawString("Wifi Connect Lost",120,60,2);}
    if (WiFi.status() == WL_DISCONNECTED){tft.drawString("Wifi Disconnected",120,60,2);}
    if (serveren){tft.drawString("SD Webserver ON",120,80,2);}else{tft.drawString("SD Webserver OFF",120,80,2);}
  }
  if(substate == 2){
    WifiTimeSync();
  }
  if(substate == 3){
    SDWebServer();
  }
}
