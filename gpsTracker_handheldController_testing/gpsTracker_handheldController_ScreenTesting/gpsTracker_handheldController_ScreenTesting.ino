#define sclk 13 // Don't change
#define mosi 11 // Don't change
#define cs   9
#define dc   8
#define rst  7  // you can also connect this to the Arduino reset

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_QDTech.h> // Hardware-specific library
#include <SPI.h>

#include "testMap.h"

Adafruit_QDTech tft = Adafruit_QDTech(cs, dc, rst);  // Invoke custom library

void setCursorLine(unsigned int lineX, unsigned int lineY) {
  //128px (126px used for chars) x 160px  =  21 x 20 characters
  tft.setCursor(lineX * 6, lineY * 8);
}

void setup() {
  tft.init();
  tft.setRotation(0);  // 0 = Portrait, 1 = Landscape
  tft.fillScreen(QDTech_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(QDTech_WHITE);
  tft.setTextSize(1);

  //TEST ROWS AND COLUMNS
  bool toggle = false;
  for (int x = 0; x < 21; x++) {
    setCursorLine(x, int(toggle));
    tft.print(String(x));
    toggle = !toggle;
  }
  for (int y = 0; y < 20; y++) {
    setCursorLine(0, y);
    tft.print(String(y));
  }
  delay(2000);


  //TEST LOADING BITMAP IMAGE
  tft.fillScreen(QDTech_BLACK);
  //tft.drawRect(0, 0, 128, 103, QDTech_GREY);
  tft.drawXBitmap(0, 0, myBitmap, 128, 103, QDTech_WHITE);
  tft.drawFastHLine(0, 103, 128, QDTech_WHITE);
  tft.setCursor(0, 0);


  //FILL IN TEXT
  //tft.fillScreen(QDTech_BLACK);
  setCursorLine(0, 13);
  tft.println("Sent:" "<datatype>:<data>");
  tft.println("Rcvd:" "<data>");
  tft.println("Lat: " + String(51.510357));   //latitude
  tft.println("Long:" + String(-0.116773, 3));//longitude

  setCursorLine(0, 17);
  tft.print  ("Sats:" + String(10));         //number of satellites
  setCursorLine(10, 17);
  tft.print  ("Acc: " + String(100));         //accuracy

  setCursorLine(0, 18);
  tft.println("Sped:" + String(100));        //speed
  setCursorLine(10, 18);
  tft.print  ("Dir: " "ESE");               //Direction (card)

  setCursorLine(0, 19);
  tft.println("Age: " + String(1000));        //fix age
  setCursorLine(10, 19);
  tft.println("Chck:" + String(1));          //checksum
}

void loop() {

}

