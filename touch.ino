int touchX, touchY;
bool chsc6x_is_pressed(void)
{
    if(digitalRead(TOUCH_INT) != LOW) {
        delay(3);
        if(digitalRead(TOUCH_INT) != LOW)
        return false;
    }
    return true;
}

void chsc6x_convert_xy(uint8_t *x, uint8_t *y)
{
    uint8_t x_tmp = *x, y_tmp = *y, _end = 0;
    for(int i=1; i<=2; i++){
        x_tmp = *x;
        y_tmp = *y;
        _end = (i % 2) ? SCREEN_WIDTH : SCREEN_HEIGHT;
        *x = y_tmp;
        *y = _end - x_tmp;
    }
}

void chsc6x_get_xy(int * x, int * y)
{
    uint8_t temp[CHSC6X_READ_POINT_LEN] = {0};
    uint8_t read_len = Wire.requestFrom(CHSC6X_I2C_ID, CHSC6X_READ_POINT_LEN);
    if(read_len == CHSC6X_READ_POINT_LEN){
        Wire.readBytes(temp, read_len);
        if (temp[0] == 0x01) {
        chsc6x_convert_xy(&temp[2], &temp[4]);
        *x = temp[2];
        *y = temp[4];
        }
    }
}

int Interface::touchZone (){
  if  (!chsc6x_is_pressed()){ return -1;}
  chsc6x_get_xy(&touchX, &touchY);
  int T = 50;
  if(T<touchX && touchX<240-T && T<touchY && touchY<240-T){ return 0;}
  if(touchY<touchX && 240-touchY>touchX){ return 1;}
  if(touchY<touchX && 240-touchY<touchX){ return 2;}
  if(touchY>touchX && 240-touchY<touchX){ return 3;}
  if(touchY>touchX && 240-touchY>touchX){ return 4;}
  return 0;
}

void Interface::touchTick (){
  //Touch Inputs
  int t = touchZone();
  if (t == 0 && touchLast == -1 && centTouch == 0){centTouch = 1;touchLast = t;}
  if (t == -1 && touchLast == 0 && centTouch == 1){centTouch = 2;}
  if (t > 0){
    if (t == 1 && touchLast == 3){if(state<1){state+=1;substate=0;tft.fillCircle(120,120,118,TFT_BLACK);}touchLast = t;}
    if (t == 3 && touchLast == 1){if(state>0){state-=1;substate=0;tft.fillCircle(120,120,118,TFT_BLACK);}touchLast = t;}
    if (t == 2 && touchLast == 4){substate-=1;tft.fillCircle(120,120,118,TFT_BLACK);touchLast = t;}
    if (t == 4 && touchLast == 2){substate+=1;tft.fillCircle(120,120,118,TFT_BLACK);touchLast = t;}
    if (touchLast == -1){touchLast = t;}
  }
  if (t == -1){touchLast = -1;}
  else {sleepTime = millis() + 8192;} //Update sleep time after last touch
}
