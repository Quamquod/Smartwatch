void Interface::begin () {
  pinMode(D0, INPUT);
  pinMode(TOUCH_INT, INPUT_PULLUP);
  Wire.begin();
  SD.begin(D2);
  rtc.begin();
  rtc.getTime(&timeStruct);
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  //face.loadFont(MajorMonoDisplay);
  targetTime = millis();
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
      services();
    }
    laststate=state;
    tft.drawNumber(state,120,60,4);
    if (ls != timeStruct.seconds){tft.drawString((char *)String(String((timeStruct.seconds-timeStruct.seconds%10)/10,DEC)+String(timeStruct.seconds%10,DEC)).c_str(), 180, 182, 6);ls = timeStruct.seconds;}
    battery();
  }
  int t = touchZone();
  if (t > 0){
    if (t == 1 && touchLast == 3){if(state<1){state+=1;tft.fillCircle(120,120,118,TFT_BLACK);}touchLast = t;}
    if (t == 3 && touchLast == 1){if(state>0){state-=1;tft.fillCircle(120,120,118,TFT_BLACK);}touchLast = t;}
    if (t == 2 && touchLast == 4){tft.drawString("RIGHT",120,60,4);touchLast = t;}
    if (t == 4 && touchLast == 2){tft.drawString("LEFT",120,60,4);touchLast = t;}
    if (touchLast == -1){touchLast = t;}
  }
  if (t == -1){ touchLast = -1;}
}

void Interface::watchFace () {
  if (lh != timeStruct.hours){tft.drawString((char *)String(String((timeStruct.hours-timeStruct.hours%10)/10,DEC)+String(timeStruct.hours%10,DEC)).c_str(), 60, 120,8);lh = timeStruct.hours;}
  if (lm != timeStruct.minutes){tft.drawString((char *)String(String((timeStruct.minutes-timeStruct.minutes%10)/10,DEC)+String(timeStruct.minutes%10,DEC)).c_str(), 180, 120,8);lm = timeStruct.minutes;}
  if (ls != timeStruct.seconds){tft.drawString((char *)String(String((timeStruct.seconds-timeStruct.seconds%10)/10,DEC)+String(timeStruct.seconds%10,DEC)).c_str(), 180, 182, 6);ls = timeStruct.seconds;}
}

void Interface::services () {
  tft.drawString("Services",120,40,2);
}

void Interface::battery () {
  //Vmax 2975 (4,2V);  2900 (4,1V) - 100% ,  2300 (3,275V) 0% ,empty 2100 (3V); (vBat - 2300)/600*360 ; 
  int sum = 0;
  for (int i=0; i<19; i++){
    vBat[i]=vBat[i+1];
    sum+=vBat[i]; 
  }
  vBat[19]=analogRead(D0);
  //if ((sum+vBat[19]) > 58000){tft.drawNumber(100,120,30,4);}
  //else if ((sum+vBat[19]) < 56000){tft.drawNumber(100,120,30,4);}
  //else {tft.drawNumber(((sum+vBat[19])/20-2300)/6,120,30,4);}
  uint32_t endAngle = ((sum+vBat[9])/20 - 2300)*3/5;
  if ((sum+vBat[19])/20 < 2300){tft.drawArc(120, 120, 120, 119, 0, 360, TFT_RED, TFT_BLACK);}
  else if (endAngle < 45){tft.drawArc(120, 120, 120, 119, 0, endAngle, TFT_RED, TFT_BLACK);}
  else if (endAngle < 105){tft.drawArc(120, 120, 120, 119, 0, endAngle, TFT_YELLOW, TFT_BLACK);}
  else {tft.drawArc(120, 120, 120, 119, 0, endAngle, TFT_GREEN, TFT_BLACK);}
  if (endAngle < 360 && (sum+vBat[9])/10 > 2300){tft.drawArc(120, 120, 120, 119, endAngle, 0, TFT_BLACK, TFT_BLACK);}
}

int Interface::touchZone (){
  if  (!chsc6x_is_pressed()){ return -1;}
  chsc6x_get_xy(&touchX, &touchY);
  int X = 240 - touchX;
  int Y = 240 - touchY;
  int T = 40;
  if(T<X && X<240-T && T<Y && Y<240-T){ return 0;}
  if(Y<X && 240-Y>X){ return 1;}
  if(Y<X && 240-Y<X){ return 2;}
  if(Y>X && 240-Y<X){ return 3;}
  if(Y>X && 240-Y>X){ return 4;}
  return 0;
}
