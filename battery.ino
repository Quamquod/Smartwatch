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
