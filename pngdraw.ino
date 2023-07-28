File myfile;
int16_t xposimg, yposimg;
PNG png;
#define MAX_IMAGE_WIDTH 240

void Interface::drawPNG(int16_t x, int16_t y, const char *name){
  xposimg = x;
  yposimg = y;
  //int16_t rc = png.openFLASH((uint8_t *)image, sizeimg, pngDraw);
  int16_t rc = png.open((const char *)name, myOpen, myClose, myRead, mySeek, pngDraw);
  if (rc == PNG_SUCCESS) {
    //tft.startWrite();
    rc = png.decode(NULL, 0);
    //tft.endWrite();
    png.close(); // Required for files, not needed for FLASH arrays
    //tft.endWrite();
  }
}

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];          // Line buffer for rendering
  uint8_t  maskBuffer[1 + MAX_IMAGE_WIDTH / 8];  // Mask buffer

  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);

  if (png.getAlphaMask(pDraw, maskBuffer, 255)) {
    // Note: pushMaskedImage is for pushing to the TFT and will not work pushing into a sprite
    UI.tft.pushMaskedImage(xposimg, yposimg + pDraw->y, pDraw->iWidth, 1, lineBuffer, maskBuffer);
  }
}
/*
void PNGDraw(PNGDRAW *pDraw) {
uint16_t usPixels[320];

  png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
  UI.tft.writeRect(0, pDraw->y + 24, pDraw->iWidth, 1, usPixels);
}*/
void * myOpen(const char *filename, int32_t *size) {
  //Serial.printf("Attempting to open %s\n", filename);
  myfile = SD.open(filename);
  *size = myfile.size();
  return &myfile;
}
void myClose(void *handle) {
  if (myfile) myfile.close();
}
int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  if (!myfile) return 0;
  return myfile.read(buffer, length);
}
int32_t mySeek(PNGFILE *handle, int32_t position) {
  if (!myfile) return 0;
  return myfile.seek(position);
}
