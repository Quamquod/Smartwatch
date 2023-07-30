//Wifi
#include <Dictionary.h>
const char * soft_ap_ssid = "Smartwatch_AP";
const char * soft_ap_password = "mywifi_psswd";

void Interface::WifiInit(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.enableAP(false);
  Dictionary &wifilist = *(new Dictionary());
  File wl = SD.open("/data/wifi.txt");
  wifilist.jload(wl);
  wl.close();
  for (int i = 0; i < wifilist.count(); i++){
    wifiMulti.addAP(wifilist(i).c_str(),wifilist[i].c_str());
  }
}

void Interface::WifiToggle(){
  if (wifien == false){
    wifien = true;
    wifiMulti.run(7200);
    wifiTime = millis() + 30000;
    //Wifi begin
  }
  else if (wifien == true){
    wifien = false;
    //Wifi stop
    WiFi.disconnect();
  }
}
void Interface::HotspotToggle(){
  if (hotspoten == false){
      WiFi.enableAP(true);
      WiFi.softAP(soft_ap_ssid, soft_ap_password);
      hotspoten = true;
  }
  else if (hotspoten == true){
      WiFi.enableAP(false);
      hotspoten = false;
  }
}
