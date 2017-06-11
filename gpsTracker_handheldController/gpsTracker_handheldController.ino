#define MAP_WIDTH_PX 128 // Height of the map on the screen in pixels
#define MAP_HEIGHT_PX 103 // Width of the map on the screen in pixels

#define MAP_MIN_LAT  51.49933 // Bottom of map
#define MAP_MAX_LAT  51.50922 // Top of map
#define MAP_MIN_LONG -0.14816 // Left of map
#define MAP_MAX_LONG -0.12059 // Right of map

#define COLOUR_BACKGROUND QDTech_BLACK // Display background colour
#define COLOUR_BITMAP QDTech_WHITE // Colour of the map
#define COLOUR_TEXT QDTech_WHITE // Colour of the text
#define COLOUR_MAP_BORDER QDTech_WHITE // The colour used for the line which separates the map from the text below
#define COLOUR_LOCATION_CURRENT QDTech_RED // Colour of the pixel used to represent the current location on the map
#define COLOUR_LOCATION_HISTORY QDTech_GREEN // The colour of the line showing location history

#define TFT_ORIENTATION 0 // Orientation of TFT LCD 1.8" screen. 0 = Portrait, 1 = Landscape
#define UART_SERIAL_BAUD 1200 // Baud rate of the UART serial port (used for debugging)
#define SS_SERIAL_BAUD 1200 // Baud rate of the software serial port (used for wireless communication)
#define HCV_READVCC_INTERVAL 30000 // Handheld controller readVcc() interval in ms

#define CELLVOLTS_BUTTON 4
#define REQUEST_GPS_ONCE_BUTTON 5
#define REQUEST_GPS_CONTINUOUS_BUTTON 6
#define REQUEST_GPS_STOP_BUTTON 7
#define REDRAW_MAP_BUTTON 8
#define LED_PIN 9

#define sclk 13 // Don't change
#define mosi 11 // Don't change
#define cs   10
#define dc   A0
#define rst  A1 // You can also connect this to the Arduino reset

#define _SS_MAX_RX_BUFF 128
#define SOFTWARE_SERIAL_RX 3
#define SOFTWARE_SERIAL_TX 2

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_QDTech.h> // Hardware-specific library
#include <SPI.h>
#include <SoftwareSerial.h>

//Include the bitmap of the map. Location of the GPS tracker is drawn on the map
#include "mapBitmap.h"

SoftwareSerial ss(SOFTWARE_SERIAL_RX, SOFTWARE_SERIAL_TX); // For HC-12 wireless communication module.

Adafruit_QDTech tft = Adafruit_QDTech(cs, dc, rst);

int previousCoordinates[] = { -1, -1};
unsigned long printHCVTargetTime;

void transmit(String datatype, String data) {
  setCursorLine(5, 13);
  tft.print(datatype + ":" + data);
  ss.print(datatype + ":" + data + "\n");
  delay(1000);
  clearTFTTransmitted();
}

void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
}

bool digitalReadDebounce(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(50);
    bool debounceRunning = true;
    while (debounceRunning) {
      if (digitalRead(pin) == HIGH) {
        debounceRunning = false;
      }
    }
    delay(50);
    return true;
  }
  return false;
}

void setCursorLine(unsigned int lineX, unsigned int lineY) {
  //Screen is 128px x 160px
  //Characters are 6px x 8px
  //(126px used for chars) x 160px  =  21 x 20 characters
  tft.setCursor(lineX * 6, lineY * 8);
}

void clearTFTGPS() {
  //  fillRect(x0,    y0,     w,      h,     colour)
  tft.fillRect(5 * 6, 15 * 8, 17 * 6, 2 * 8, COLOUR_BACKGROUND); //clear lat and long
  tft.fillRect(5 * 6, 17 * 8, 6 * 6, 2 * 8, COLOUR_BACKGROUND); //clear bottom left values, except the bottom row - which is for cell voltage
  tft.fillRect(15 * 6, 17 * 8, 7 * 6, 2 * 8, COLOUR_BACKGROUND); //clear bottom right values, except the bottom row - which is for cell voltage
}

void clearTFT_HCV() { // Clear handheld controller voltage
  tft.fillRect(5 * 6, 19 * 8, 6 * 6, 1 * 8, COLOUR_BACKGROUND); //clear HCV value (handheld controller voltage)
}

void clearTFT_TDV() { // Clear tracking device voltage
  tft.fillRect(15 * 6, 19 * 8, 7 * 6, 1 * 8, COLOUR_BACKGROUND); //clear TDV value (tracking device voltage)
}

void clearTFTReceived() {
  tft.fillRect(5 * 6, 14 * 8, 17 * 6, 1 * 8, COLOUR_BACKGROUND); //clear the received text
}

void clearTFTTransmitted() {
  tft.fillRect(5 * 6, 13 * 8, 17 * 6, 1 * 8, COLOUR_BACKGROUND); // Clear the transmitted text
}

void clearTFTMap() {
  tft.fillRect(0, 0, 128, 103, COLOUR_BACKGROUND); //clear the map drawn on the TFT screen
}

void drawTFTMap() {
  tft.drawXBitmap(0, 0, mapBitmap, MAP_WIDTH_PX, MAP_HEIGHT_PX, COLOUR_BITMAP); //Draw bitmap image
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
      tft.fillTriangle(screenX, screenY, screenX + 2, screenY + 5, screenX + 5, screenY + 2, COLOUR_LOCATION_CURRENT);
      break;
    case 1: //N
      tft.fillTriangle(screenX, screenY, screenX - 2, screenY + 5, screenX + 2, screenY + 5, COLOUR_LOCATION_CURRENT);
      break;
    case 2: //NE
      tft.fillTriangle(screenX, screenY, screenX - 2, screenY + 5, screenX - 5, screenY + 2, COLOUR_LOCATION_CURRENT);
      break;
    case 3: //E
      tft.fillTriangle(screenX, screenY, screenX - 5, screenY - 2, screenX - 5, screenY + 2, COLOUR_LOCATION_CURRENT);
      break;
    case 4: //SE
      tft.fillTriangle(screenX, screenY, screenX - 2, screenY - 5, screenX - 5, screenY - 2, COLOUR_LOCATION_CURRENT);
      break;
    case 5: //S
      tft.fillTriangle(screenX, screenY, screenX + 2, screenY - 5, screenX - 2, screenY - 5, COLOUR_LOCATION_CURRENT);
      break;
    case 6: //SW
      tft.fillTriangle(screenX, screenY, screenX + 2, screenY - 5, screenX + 5, screenY - 2, COLOUR_LOCATION_CURRENT);
      break;
    case 7: //W
      tft.fillTriangle(screenX, screenY, screenX + 5, screenY + 2, screenX + 5, screenY - 2, COLOUR_LOCATION_CURRENT);
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
    //Draw pixel on map at previous coordinates for plot of location history
    tft.drawPixel(previousCoordinates[0], previousCoordinates[1], COLOUR_BACKGROUND); //Clear the pixel first
    tft.drawPixel(previousCoordinates[0], previousCoordinates[1], COLOUR_LOCATION_HISTORY);

    //Draw pixel on map at current location
    tft.drawPixel(screenXPos, screenYPos, COLOUR_BACKGROUND); //Clear the pixel first
    tft.drawPixel(screenXPos, screenYPos, COLOUR_LOCATION_CURRENT);

    //Uncomment to draw a '+' of pixels around location (May break line on graph showing tracker's location history)
    //tft.drawPixel(screenXPos + 1, screenYPos    , COLOUR_LOCATION_CURRENT);
    //tft.drawPixel(screenXPos - 1, screenYPos    , COLOUR_LOCATION_CURRENT);
    //tft.drawPixel(screenXPos    , screenYPos + 1, COLOUR_LOCATION_CURRENT);
    //tft.drawPixel(screenXPos    , screenYPos - 1, COLOUR_LOCATION_CURRENT);

    //Uncomment to draw a 'x' of pixels around location (May break line on graph showing tracker's location history)
    //tft.drawPixel(screenXPos-1, screenYPos-1, COLOUR_LOCATION_CURRENT);
    //tft.drawPixel(screenXPos-1, screenYPos+1, COLOUR_LOCATION_CURRENT);
    //tft.drawPixel(screenXPos+1, screenYPos+1, COLOUR_LOCATION_CURRENT);
    //tft.drawPixel(screenXPos+1, screenYPos-1, COLOUR_LOCATION_CURRENT);

    //Set previousCoordinates to current location for next time
    previousCoordinates[0] = screenXPos;
    previousCoordinates[1] = screenYPos;

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

long readVcc() {
  // Function to read Arduino's Vcc voltage.
  // Inspiration taken from https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
  // For ATMEGA168 and ATMEGA328 where the internal reference voltage is 1.1 volts. i.e Arduino Uno, nano and Pro Mini should work fine
  // Internal reference voltage for the ATMEGA8 is 2.56 volts, so it should be easy to modify this code for the ATMEGA8
  // The Arduino Mega has two internal voltage references at 1.1v and 2.56v. There's a chance this code could changed to work with the Arduino Mega
  // The voltage is returned in millivolts. So 5000 is 5V, 3300 is 3.3V

  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(CELLVOLTS_BUTTON, INPUT_PULLUP);
  pinMode(REQUEST_GPS_ONCE_BUTTON, INPUT_PULLUP);
  pinMode(REQUEST_GPS_CONTINUOUS_BUTTON, INPUT_PULLUP);
  pinMode(REQUEST_GPS_STOP_BUTTON, INPUT_PULLUP);
  pinMode(REDRAW_MAP_BUTTON, INPUT_PULLUP);

  //Serial.begin(UART_SERIAL_BAUD); //For debugging
  ss.begin(SS_SERIAL_BAUD); //For HC-12 wireless communication module

  tft.init();
  tft.setRotation(TFT_ORIENTATION);
  tft.fillScreen(COLOUR_BACKGROUND);
  tft.setTextWrap(false);
  tft.setCursor(0, 0);
  tft.setTextColor(COLOUR_TEXT);
  tft.setTextSize(1);

  //DRAW BITMAP IMAGE AND PRINT VALUE TITLES
  drawTFTMap();
  tft.drawFastHLine(0, 103, 128, COLOUR_MAP_BORDER); //Note: draw on line 103, which is the 104th line
  setCursorLine(0, 13); //Print titles for values
  tft.println("Sent:");
  tft.println("Rcvd:");
  tft.println("Lat: ");
  tft.println("Long:");
  tft.print("Sats:");
  setCursorLine(11, 17);
  tft.println("Acc:");
  tft.print("Sped:");
  setCursorLine(11, 18);
  tft.println("Dir:");
  //  tft.print("Age:");
  //  setCursorLine(11, 19);
  //  tft.print("Chck:");
  tft.print("HCV:");
  setCursorLine(11, 19);
  tft.print("TDV:");

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (printHCVTargetTime < millis()) {
    clearTFT_HCV(); // Clear any old tracking device voltage values
    setCursorLine(5, 19); // Move cursor
    tft.print(String(readVcc()) + "mV"); // Print voltage in mV
    printHCVTargetTime = millis() + HCV_READVCC_INTERVAL;
  }
  if (digitalReadDebounce(CELLVOLTS_BUTTON)) {
    transmit("cell", "volts");
  }
  if (digitalReadDebounce(REQUEST_GPS_ONCE_BUTTON)) {
    transmit("gps", "once");
  }
  if (digitalReadDebounce(REQUEST_GPS_CONTINUOUS_BUTTON)) {
    transmit("gps", "continuous");
  }
  if (digitalReadDebounce(REQUEST_GPS_STOP_BUTTON)) {
    transmit("gps", "stop");
  }
  if (digitalReadDebounce(REDRAW_MAP_BUTTON)) {
    clearTFTMap(); //Clear the map
    drawTFTMap();  //Re-draw the map
  }

  if (ss.available() > 0) {
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    //// PROCESSING INCOMING SERIAL DATA from "<datatype>:<csv of data>" INTO 'datatype' AND 'datacsvarray' ////
    //Read the incoming byte:
    String receivedSerialString = ss.readStringUntil('\n');
    setCursorLine(5, 14);
    tft.print(String(receivedSerialString));

    //For debugging
    //Serial.println("received: " + receivedSerialString);
    //Serial.println("length: " + String(sizeof(receivedSerialString)));
    //if (ss.overflow()) {
    //  Serial.println("SoftwareSerial overflow!");
    //} else {
    //  Serial.println("no SoftwareSerial overflow!");
    //}

    //Split receivedSerialString into datatype and data
    int indexofcolon = receivedSerialString.indexOf(":");
    int lengthofstring = receivedSerialString.length();
    String datatype = "";
    if (indexofcolon == -1) {
      //Transmit("message", "error - no colon in string");
      delay(100); //Do nothing
    } else if (indexofcolon != 0 && indexofcolon != lengthofstring - 1) {
      datatype = receivedSerialString.substring(0, indexofcolon);
      String datacsv = receivedSerialString.substring(indexofcolon + 1);
      int indexofcomma = datacsv.indexOf(",");
      //For every character in datacsv, if it is a comma add 1 to commacount
      //If not a comma then add 0 to commacount (so commacount doesn't increase)
      int i, commacount;
      for (i = 0, commacount = 0; datacsv[i]; i++) {
        commacount += (datacsv[i] == ',');
      }
      //Create an array with 1 more element than the amount of commas
      //e.g. "1,2,3" has 2 commas and 3 pieces of information
      String datacsvarray[commacount + 1];
      //For the amount of commas+1 in datacsv:
      for (int i = 0; i < commacount + 1; i++) {
        //if there's a comma in datacsv
        if (datacsv.indexOf(",") > -1) {
          //i's position in datacsvarray is set to the string between the start of datacsv and the next comma
          datacsvarray[i] = datacsv.substring(0, datacsv.indexOf(","));
          //The string between the start of datacsv up to and including the next comma is removed from the datacsv
          datacsv.remove(0, datacsv.indexOf(",") + 1);
        } else {
          //i's position in datacsvarray is set to datacsv. This happens once at the end of the for loop when all commas have been removed from datacsv
          datacsvarray[i] = datacsv;
          //For consistency, delete the reset of datacsv
          datacsv.remove(0);
        }
      }

      //For debugging
      //Serial.println("data type: " + datatype);
      //Serial.println("Comma count: " + String(commacount));
      //Serial.println("sizeof datacsvarray: " + String(sizeof(datacsvarray)));
      //for (int i = 0; i <= commacount; i++) {
      //  Serial.print("Data " + String(i) + " " + datacsvarray[i]);
      //  Serial.println(",");
      //}
      //// PROCESSING 'datatype' AND 'datacsvarray' ////
      if (datatype == "cell") {
        clearTFT_TDV(); // Clear any old tracking device voltage values
        setCursorLine(15, 19); // Move cursor
        tft.print(datacsvarray[0]); // Print voltage in mV
      } else if (datatype == "gps") {
        //data = "latitude,longitude,num of satellites,accuracy,speed,direction,age of data,checksum"
        clearTFTGPS();

        setCursorLine(5, 15);
        tft.print(datacsvarray[0]); //latitude
        setCursorLine(5, 16);
        tft.print(datacsvarray[1]); //longitude
        setCursorLine(5, 17);
        tft.print(datacsvarray[2]); //num of satellites
        setCursorLine(15, 17);
        tft.print(datacsvarray[3]); //accuracy
        setCursorLine(5, 18);
        tft.print(datacsvarray[4]); //speed
        setCursorLine(15, 18);
        tft.print(datacsvarray[5]); //direction
        //setCursorLine(5, 19);
        //tft.print(datacsvarray[6]); //age of data
        //setCursorLine(15, 19);
        //tft.print(datacsvarray[7]); //checksum

        if (commacount != 7) { // If incorrect number of commas received, show it in bottom right corner of screen
          setCursorLine(20, 19);
          tft.print(commacount);
        }

        clearTFTMap(); //Clear the map
        drawTFTMap();  //Re-draw the map
        displayLocationOnMap(datacsvarray[0].toFloat(), datacsvarray[1].toFloat()); //Display current location on the map. Arguments are (float latitude, float longitude)

      } else if (datatype == "message") {
        clearTFTReceived(); //message is written to the same area of the screen as received text, so this is cleared first
        setCursorLine(5, 14);
        tft.print(datacsvarray[0]); //For messages with only one piece of data and therefore one item in datacsvarray[]
        /*for (int i = 0; i <= commacount; i++) { //For messages with multiple items in datacsvarray[] (not used)
              tft.print(datacsvarray[i]);
              tft.print(",");
            }*/
        delay(1000); //Delay for a second so the user can read the message
      } else {
        //transmit("message", "message datatype not understood");
        delay(100); //Do nothing
      }
    } else {
      transmit("message", "nothing on one side of colon character");
    }
    delay(200);
    clearTFTReceived();
    digitalWrite(LED_PIN, LOW);
  }
}

