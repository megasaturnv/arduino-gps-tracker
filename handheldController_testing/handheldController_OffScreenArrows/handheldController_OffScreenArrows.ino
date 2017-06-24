#define MAP_WIDTH_PX 128
#define MAP_HEIGHT_PX 103

#define MAP_MIN_LAT  -0.5 //bottom of map
#define MAP_MAX_LAT   0.5 //top of map
#define MAP_MIN_LONG -0.5 //left of map
#define MAP_MAX_LONG  0.5 //right of map

#define sclk 13 // Don't change
#define mosi 11 // Don't change
#define cs   10
#define dc   A0
#define rst  A1 // you can also connect this to the Arduino reset

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_QDTech.h> // Hardware-specific library
#include <SPI.h>

Adafruit_QDTech tft = Adafruit_QDTech(cs, dc, rst);

//2D array of fake locations outside map centered around 0 lat, 0 long
//fakeLocations[latitude][longitude]. i.e. fakeLocations[y coord][x coord]
const float fakeLocations[16][2] = {
  { 1, -1 },
  { 1, -0.25 },
  { 1, 0 },
  { 1, 0.25 },

  { 1,     1},
  { 0.25,  1 },
  { 0,     1},
  { -0.25, 1},

  { -1, 1},
  { -1, 0.25},
  { -1, 0},
  { -1, -0.25},

  { -1,    -1 },
  { -0.25, -1 },
  { 0,     -1 },
  { 0.25,  -1 },
};

void clearTFTMap() {
  tft.fillRect(0, 0, 128, 103, QDTech_BLACK); //clear the map drawn on the TFT screen
}

void drawArrow(byte screenX, byte screenY, byte dir) {
  //dir is the direction of the arrow. North-West is 0, North is 1, North east is 2 such that:
  //  NW  N  NE     0 1 2
  //   W     E   =  7   3
  //  SW  S  SE     6 5 4

  //Note: Dir could be inferred from screenX and screenY...
  switch (dir) {
    case 0: //NW
      //  fillTriangle(x0,      y0,      x1,          y1,          x2,          y2,          colour)
      tft.fillTriangle(screenX, screenY, screenX + 2, screenY + 5, screenX + 5, screenY + 2, QDTech_RED);
      break;
    case 1: //N
      tft.fillTriangle(screenX, screenY, screenX - 2, screenY + 5, screenX + 2, screenY + 5, QDTech_RED);
      break;
    case 2: //NE
      tft.fillTriangle(screenX, screenY, screenX - 2, screenY + 5, screenX - 5, screenY + 2, QDTech_RED);
      break;
    case 3: //E
      tft.fillTriangle(screenX, screenY, screenX - 5, screenY - 2, screenX - 5, screenY + 2, QDTech_RED);
      break;
    case 4: //SE
      tft.fillTriangle(screenX, screenY, screenX - 2, screenY - 5, screenX - 5, screenY - 2, QDTech_RED);
      break;
    case 5: //S
      tft.fillTriangle(screenX, screenY, screenX + 2, screenY - 5, screenX - 2, screenY - 5, QDTech_RED);
      break;
    case 6: //SW
      tft.fillTriangle(screenX, screenY, screenX + 2, screenY - 5, screenX + 5, screenY - 2, QDTech_RED);
      break;
    case 7: //W
      tft.fillTriangle(screenX, screenY, screenX + 5, screenY + 2, screenX + 5, screenY - 2, QDTech_RED);
      break;
  }
}

void displayLocationOnMap(float currentLatitude, float currentLongitude) {
  //Relative X position on map between 0 and 1
  float relativeXPosOnMap = (currentLongitude - MAP_MIN_LONG) / (MAP_MAX_LONG - MAP_MIN_LONG);
  //Calculate screen X coordinates for current GPS location
  int screenXPos = relativeXPosOnMap * float(MAP_WIDTH_PX);

  //Relative Y position on map between 0 and 1
  //1.0 - the relative Position because Y-coordinates on screen are inversed relative to latitude (and the standard 'Cartesian' coordinate system
  //i.e. Y-coordinate increases as you go down the screen. Latitude increases as you go up the screen.
  //There are many ways to fix this problem. e.g. change MAP_MIN_LAT to MAP_MAX_LAT or switching the denominators should have the same result
  float relativeYPosOnMap = 1.0 - ((currentLatitude - MAP_MIN_LAT) / (MAP_MAX_LAT - MAP_MIN_LAT));
  //Calculate screen Y coordinates for current GPS location.
  int screenYPos = relativeYPosOnMap * float(MAP_HEIGHT_PX);

  //For debugging
  //Serial.println("RelXPos: " + String(relativeXPosOnMap));
  //Serial.println("RelYPos: " + String(relativeYPosOnMap));
  //Serial.println("MapXPos: " + String(screenXPos));
  //Serial.println("MapYPos: " + String(screenYPos));

  //if tracker is on-screen
  if (relativeXPosOnMap >= 0 && relativeXPosOnMap <= 1 && relativeYPosOnMap >= 0 && relativeYPosOnMap <= 1) {
    //Draw pixel on map at current location
    tft.drawPixel(screenXPos, screenYPos, QDTech_BLACK); //Clear the pixel first
    tft.drawPixel(screenXPos, screenYPos, QDTech_RED);

    ////if tracker is off-screen////
    ////corners of map. i.e. position is completely outside the map's boundaries
    //if tracker is top-left
  } else if (relativeXPosOnMap < 0 && relativeYPosOnMap < 0) {
    drawArrow(0, 0, 0);
    //if tracker is top-right
  } else if (relativeXPosOnMap > 1 && relativeYPosOnMap < 0) {
    drawArrow(MAP_WIDTH_PX, 0, 2);
    //if tracker is bottom-right
  } else if (relativeXPosOnMap > 1 && relativeYPosOnMap > 1) {
    drawArrow(MAP_WIDTH_PX, MAP_HEIGHT_PX - 1, 4);
    //if tracker is bottom-left
  } else if (relativeXPosOnMap < 0 && relativeYPosOnMap > 1) {
    drawArrow(0, MAP_HEIGHT_PX - 1, 6);

    ////edges of map. i.e. position is outside the map in one axis but within the map on the other axis
    //if tracker is top-middle
  } else if (relativeXPosOnMap >= 0 && relativeXPosOnMap <= 1 && relativeYPosOnMap < 0) {
    drawArrow(screenXPos, 0, 1);
    //if tracker is middle-right
  } else if (relativeXPosOnMap > 1 && relativeYPosOnMap <= 1 && relativeYPosOnMap >= 0) {
    drawArrow(MAP_WIDTH_PX, screenYPos, 3);
    //if tracker is bottom-middle
  } else if (relativeXPosOnMap >= 0 && relativeXPosOnMap <= 1 && relativeYPosOnMap > 1) {
    drawArrow(screenXPos, MAP_HEIGHT_PX - 1, 5);
    //if tracker is middle-left
  } else if (relativeXPosOnMap < 0 && relativeYPosOnMap <= 1 && relativeYPosOnMap >= 0) {
    drawArrow(0, screenYPos, 7);
  }
}

void setup() {
  tft.init();
  tft.setRotation(0);  // 0 = Portrait, 1 = Landscape
  tft.fillScreen(QDTech_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(QDTech_WHITE);
  tft.setTextSize(1);
  tft.drawFastHLine(0, 103, 128, QDTech_WHITE); //Note: draw on line 103, which is the 104th line

  //Draw arrows rotating in centre of map
  //With clearing the map
  for (byte i = 0; i <= 7; i++) {
    clearTFTMap(); //Clear the map
    drawArrow(MAP_WIDTH_PX / 2, MAP_HEIGHT_PX / 2, i);
    delay(200);
  }
  delay(1000);

  //Without clearing the map - for arrow comparison
  clearTFTMap(); //Clear the map
  for (byte i = 0; i <= 7; i++) {
    drawArrow(MAP_WIDTH_PX / 2, (i + 1) * 11, i);
    delay(200);
  }
  delay(1000);

  //Draw arrows in corners with drawArrow()
  clearTFTMap(); //Clear the map
  drawArrow(0, 0, 0);
  delay(500);
  drawArrow(MAP_WIDTH_PX, 0, 2);
  delay(500);
  drawArrow(MAP_WIDTH_PX, MAP_HEIGHT_PX, 4);
  delay(500);
  drawArrow(0, MAP_HEIGHT_PX, 6);
  delay(1000);

  //Draw arrows in corners with displayLocationOnMap(float latitude, float longitude)
  //Should be the same result as above
  clearTFTMap(); //Clear the map
  displayLocationOnMap(1.0, -1.0);
  delay(500);
  displayLocationOnMap(1.0, 1.0);
  delay(500);
  displayLocationOnMap(-1.0, 1.0);
  delay(500);
  displayLocationOnMap(-1.0, -1.0);
  delay(1000);

  //Draw test locations from fakeLocations 2D array on map
  clearTFTMap(); //Clear the map
  for (byte i = 0; i < 16; i++) {
    //displayLocationOnMap(float latitude, float longitude)
    displayLocationOnMap(fakeLocations[i][0], fakeLocations[i][1]);
    //tft.println(String(fakeLocations[i][0]) + "," + String(fakeLocations[i][1]));
    delay(200);
  }
}

void loop() {

}

