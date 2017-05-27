#include <avr/power.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
//https://github.com/mikalhart/TinyGPS

#define SOFTWARE_SERIAL_RX 3
#define SOFTWARE_SERIAL_TX 2
#define GPS_TIMEOUT 0 //Amount of time in ms until the Arduino gives up trying to receive a GPS signal
//Set to 0 to disable timeout
#define MAX_ON_TIME 600000 //Failsafe maximum on-time in ms for the Arduino. MAX_ON_TIME will reset when awaking or if the arduino receives data in its serial buffer
//Set to 0 to disable failsafe sleep

TinyGPS gps;
SoftwareSerial ss(SOFTWARE_SERIAL_RX, SOFTWARE_SERIAL_TX); //For GPS module

const byte OutputPowerSavingPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}; // Unused pins that should be set as outputs (to save power)

unsigned long startMillis, targetTime;

bool requestSleep = true;
byte gpsRequestMode = 0;
//0 = handheld is no requesting GPS
//1 = handheld is requesting GPS data to be transmitted once
//2 = handheld is requesting continous transmission of GPS data

void blinkLED() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
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

void transmit(String datatype, String data) {
  Serial.print(datatype + ":" + data + "\n");
  delay(1000); //TEMP - testing
}

void ArduinoSleep() {
  //Based on code from http://playground.arduino.cc/Learning/ArduinoSleepCode and http://rubenlaguna.com/wp/2008/10/15/arduino-sleep-mode-waking-up-when-receiving-data-on-the-usart/

  requestSleep = false;
  ss.end(); // Stop software serial (it was waking up the arduino for some reason
  transmit("message", "sleeping");
  delay(2000); // TEMP gives extra time for message to exit serial buffer

  set_sleep_mode(SLEEP_MODE_IDLE);   // Sleep mode is set here

  sleep_enable();          // Enables the sleep bit in the mcucr register so sleep is possible. just a safety pin

  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();

  PRR = PRR | 0b00100000; // Disable the 8-bit timer http://playground.arduino.cc/Learning/ArduinoSleepCode

  sleep_mode();            // Here the device is actually put to sleep!!

  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // First thing after waking from sleep: disable sleep...

  PRR = PRR & 0b00000000; // Re-enable the 8-bit timer. The PRR refers to the Power Reduction Register.

  power_all_enable();

  ss.begin(9600); // Begin software serial
  startMillis = millis();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(1200); //HC-12 module on Arduino serial port. Incoming data here will wake up the arduino.
  ss.begin(9600);     //GPS module on Arduino ss port (software serial)

  //Save Power by writing all Digital IO to outputs. Remember to not damage any attached devices. Max pin current ~ 20mA
  //Loop for the amount of values in OutputPowerSavingPins array.
  //In this case, sizeof(byte) == 1 so it's not really needed.
  //This is better practice when needing to know the amount of items in an int array.
  //for (byte i = 0; i < (sizeof(OutputPowerSavingPins) / sizeof(byte)); i++) {
  //  pinMode(OutputPowerSavingPins[i], OUTPUT);
  //}

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(4000);
    //// PROCESSING INCOMING SERIAL DATA from "<datatype>:<csv of data>" INTO 'datatype' AND 'datacsvarray' ////
    //Read the incoming byte:
    String receivedSerialString = Serial.readStringUntil('\n');

    //Split receivedSerialString into datatype and data
    int indexofcolon = receivedSerialString.indexOf(":");
    int lengthofstring = receivedSerialString.length();
    String datatype = "";
    if (indexofcolon == -1) {
      //Transmit("message", "error - no colon in string");
      delay(100); //Do nothing
    } else if (indexofcolon != 0 && indexofcolon != lengthofstring - 1) {
      //Data is formatted correctly
      startMillis = millis(); //Reset startMillis

      //Begin decompiling message:
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

      requestSleep = true; // default action is to request sleep

      //// PROCESSING 'datatype' AND 'datacsvarray' ////
      if (datatype == "ping") {
        if (datacsvarray[0] == "handheldsend") {
          transmit("message", "trackerreceived");
        } else {
          transmit("message", "ping invalid");
        }
      } else if (datatype == "gps") {
        //data = "latitude,longitude,num of satellites,accuracy,speed,direction,age of data,checksum"
        if (datacsvarray[0] == "once") {
          transmit("message", "received gps once");
          gpsRequestMode = 1;
          requestSleep = false;
        } else if (datacsvarray[0] == "continuous") {
          transmit("message", "received gps continuous");
          gpsRequestMode = 2;
          requestSleep = false;
        } else if (datacsvarray[0] == "stopcontinuous") {
          transmit("message", "received gps stop continuous");
          gpsRequestMode = 0;
        }
        targetTime = millis() + GPS_TIMEOUT;
      } else {
        //transmit("message", "message datatype not understood");
        delay(100); //Do nothing
      }
    } else {
      transmit("message", "nothing on one side of colon character");
    }
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }


  if (gpsRequestMode == 0) {
    requestSleep = true; //request sleep
  } else {
    if (targetTime > millis() || GPS_TIMEOUT == 0) {
      bool newData = false;
      // For ten seconds we parse GPS data and report some key values
      for (unsigned long start = millis(); millis() - start < 10000;) {
        while (ss.available()) {
          char c = ss.read();
          //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
          if (gps.encode(c)) // Did a new valid sentence come in?
            newData = true;
        }
      }

      if (newData) {
        targetTime = millis() + GPS_TIMEOUT;

        byte sats;
        unsigned short sentences, failed, acc, spd;
        unsigned long age, chars;
        float flat, flon;
        String dir;
        gps.stats(&chars, &sentences, &failed);
        gps.f_get_position(&flat, &flon, &age);

        flat   = flat               == TinyGPS::GPS_INVALID_F_ANGLE    ? 0.0    : flat;                              //Latitude
        flon   = flon               == TinyGPS::GPS_INVALID_F_ANGLE    ? 0.0    : flon;                              //Longitude
        sats   = gps.satellites()   == TinyGPS::GPS_INVALID_SATELLITES ? 0      : gps.satellites();                  //Satellites
        acc    = gps.hdop()         == TinyGPS::GPS_INVALID_HDOP       ? 0      : gps.hdop();                        //precision / accuracy
        spd    = gps.f_speed_kmph() == TinyGPS::GPS_INVALID_F_SPEED    ? 0      : gps.f_speed_kmph();                //speed
        dir    = gps.f_course()     == TinyGPS::GPS_INVALID_F_ANGLE    ? "*** " : TinyGPS::cardinal(gps.f_course()); //direction
        age    = age                == TinyGPS::GPS_INVALID_AGE        ? 0      : age;                               //age of data
        failed = failed             == 0xFFFFFFFF                      ? 255    : failed;                            //checksum

        transmit("gps",
                 String(flat, 6) + "," +
                 String(flon, 6) + "," +
                 String(sats) + "," +
                 String(acc) + "," +
                 String(spd) + "," +
                 dir + "," +
                 String(age) + "," +
                 String(failed)
                );

        if (gpsRequestMode == 1) { //GPS request mode is set to 1
          gpsRequestMode = 0; //Set gpsRequestMode to 0, which will loop around and then request sleep
        }
      }
    } else { //if Arduino can't get a GPS location after GPS_TIMEOUT ms
      transmit("message", "Unable to get GPS fix. Sleeping..."); //Tell the handheld we cannot get a GPS location
      gpsRequestMode = 0; //Stop requesting GPS data. Program will loop around and do requestSleep = true.
    }
  }
  if (millis() - startMillis > MAX_ON_TIME && MAX_ON_TIME != 0) { //Fail-safe if arduino is on for too long
    transmit("message", "Online too long without contact. Sleeping...");
    gpsRequestMode = 0;
    requestSleep = true;
    // The program would ordinarily loop back around and then set requestSleep = true because gpsRequestMode == 0
    // However, setting it manually in this case to make sure the Arduino turns off after the MAX_ON_TIME to save power
  }
  if (requestSleep == true) {
    ArduinoSleep();
  }
}

