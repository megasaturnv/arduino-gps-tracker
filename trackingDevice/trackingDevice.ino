#include <avr/power.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
//https://github.com/mikalhart/TinyGPS

#define UART_SERIAL_BAUD 1200 // Baud rate of the UART serial port (used for wireless communication)
#define SS_SERIAL_BAUD 9600 // Baud rate of the software serial port (used for GPS module communication)
#define GPS_TIMEOUT 0 // Amount of time in ms until the Arduino gives up trying to receive a GPS signal. Set to 0 to disable timeout
#define MAX_ON_TIME 3600000 // Failsafe maximum on-time in ms for the Arduino. MAX_ON_TIME will reset when awaking or if the arduino receives data in its serial buffer. Set to 0 to disable failsafe sleep
#define GPS_DATA_TRANSMIT_INTERVAL 15000 //How often data is transmitted to the handheld controller when gpsRequestMode == 2 (Continuous GPS data transmission)
#define SOFTWARE_SERIAL_RX 3 // Software serial RX pin, to be connected to the GPS module's TX
#define SOFTWARE_SERIAL_TX 2 // Software serial TX pin, to be connected to the GPS module's RX
#define GPS_POWER_PIN 4 // Control power to the GPS module via a transistor

TinyGPS gps;
SoftwareSerial ss(SOFTWARE_SERIAL_RX, SOFTWARE_SERIAL_TX); // For GPS module

// Unused pins that should be set as outputs (this slightly reduces the amount of power used by the Arduino).
// Pin 14 = A0, 15 = A2 etc to 19=A5. DO NOT plug anything into these pins.
// ################################################################
// #### YOU MUST UPDATE THIS ARRAY IF ANY PIN CHANGES ARE MADE ####
// ################################################################
const byte OutputPowerSavingPins[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

unsigned long startMillis, targetTime;

bool requestSleep = true;
byte gpsRequestMode = 0;
// The meanings of gpsRequestMode is listed below
// 0 = handheld is not requesting GPS data. Arduino should go into sleep mode
// 1 = handheld is requesting GPS data to be transmitted once. Arduino will then set gpsRequestMode to 0 and sleep
// 2 = handheld is requesting continuous transmission of GPS data

void transmit(String datatype, String data) {
  Serial.print(datatype + ":" + data + "\n");
  delay(1000); //TEMP - testing
}

void ArduinoSleep() {
  // Based on code from http://playground.arduino.cc/Learning/ArduinoSleepCode and http://rubenlaguna.com/wp/2008/10/15/arduino-sleep-mode-waking-up-when-receiving-data-on-the-usart/

  gpsRequestMode = 0; // Make sure gpsRequestMode is 0 so it sleeps if the arduino is woken by an invalid serial message
  requestSleep = false; // Set requestSleep to false so the Arduino doesn't try to sleep again
  ss.end(); // Stop software serial (it was waking up the arduino for some reason
  digitalWrite(GPS_POWER_PIN, LOW); // Turn off GPS module

  transmit("message", "sleeping");
  delay(2000); // TEMP gives extra time for message to exit serial buffer

  set_sleep_mode(SLEEP_MODE_IDLE); // Sleep mode is set here

  sleep_enable(); // Enables the sleep bit in the mcucr register so sleep is possible. just a safety pin

  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();

  PRR = PRR | 0b00100000; // Disable the 8-bit timer http://playground.arduino.cc/Learning/ArduinoSleepCode

  sleep_mode(); // Here the device is actually put to sleep!!

  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

  sleep_disable(); // First thing after waking from sleep: disable sleep...

  PRR = PRR & 0b00000000; // Re-enable the 8-bit timer. The PRR refers to the Power Reduction Register.

  power_all_enable();

  digitalWrite(GPS_POWER_PIN, HIGH); // Turn on GPS module
  delay(500); // Give GPS module half a second to turn on
  ss.begin(SS_SERIAL_BAUD); // Begin software serial for GPS module communication
  startMillis = millis(); // Record startMillis
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
  pinMode(LED_BUILTIN, OUTPUT); // Setup pin 13 (Arduino's LED pin)
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(GPS_POWER_PIN, OUTPUT); // Setup pin which controls GPS power via a transistor
  digitalWrite(GPS_POWER_PIN, LOW);

  Serial.begin(UART_SERIAL_BAUD); // HC-12 module on Arduino serial port. Incoming data here will wake up the arduino.
  ss.begin(SS_SERIAL_BAUD);     // GPS module on Arduino ss port (software serial)

  // Save Power by writing all Digital IO to outputs. Remember to not damage any attached devices. Max pin current ~ 20mA
  // Loop for the amount of values in OutputPowerSavingPins array.
  // In this case, sizeof(byte) == 1 so it's not really needed.
  // This is better practice when needing to know the amount of items in an int array.
  for (byte i = 0; i < (sizeof(OutputPowerSavingPins) / sizeof(byte)); i++) {
    pinMode(OutputPowerSavingPins[i], OUTPUT);
  }

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    //// PROCESSING INCOMING SERIAL DATA from "<datatype>:<csv of data>" INTO 'datatype' AND 'datacsvarray' ////
    // Read the incoming byte:
    String receivedSerialString = Serial.readStringUntil('\n');

    // Split receivedSerialString into datatype and data
    int indexofcolon = receivedSerialString.indexOf(":");
    int lengthofstring = receivedSerialString.length();
    String datatype = "";
    if (indexofcolon == -1) {
      // Transmit("message", "error - no colon");
      delay(100); // Do nothing
    } else if (indexofcolon != 0 && indexofcolon != lengthofstring - 1) {
      // Data is formatted correctly
      startMillis = millis(); //Reset startMillis, so Arduino doesn't call the "Fail-safe" code.
      // This turns the Arduino off if it has been on for too long without contact

      // Begin decompiling the received message:
      datatype = receivedSerialString.substring(0, indexofcolon);
      String datacsv = receivedSerialString.substring(indexofcolon + 1);
      int indexofcomma = datacsv.indexOf(",");
      // For every character in datacsv, if it is a comma add 1 to commacount
      // If not a comma then add 0 to commacount (so commacount doesn't increase)
      int i, commacount;
      for (i = 0, commacount = 0; datacsv[i]; i++) {
        commacount += (datacsv[i] == ',');
      }
      // Create an array with 1 more element than the amount of commas
      // e.g. "1,2,3" has 2 commas and 3 pieces of information
      String datacsvarray[commacount + 1];
      // For the amount of commas+1 in datacsv:
      for (int i = 0; i < commacount + 1; i++) {
        // If there's a comma in datacsv
        if (datacsv.indexOf(",") > -1) {
          // i's position in datacsvarray is set to the string between the start of datacsv and the next comma
          datacsvarray[i] = datacsv.substring(0, datacsv.indexOf(","));
          // The string between the start of datacsv up to and including the next comma is removed from the datacsv
          datacsv.remove(0, datacsv.indexOf(",") + 1);
        } else {
          // i's position in datacsvarray is set to datacsv. This happens once at the end of the for loop when all commas have been removed from datacsv
          datacsvarray[i] = datacsv;
          // For consistency, delete the reset of datacsv
          datacsv.remove(0);
        }
      }

      //// PROCESSING 'datatype' AND 'datacsvarray' ////
      if (datatype == "cell") {
        if (datacsvarray[0] == "volts") {
          transmit("cell", String(readVcc()) + "mV");
        }
      } else if (datatype == "gps") {
        // data = "latitude,longitude,num of satellites,accuracy,speed,direction,age of data,checksum"
        if (datacsvarray[0] == "once") {
          transmit("message", "rcvd gps once");
          gpsRequestMode = 1;
        } else if (datacsvarray[0] == "continuous") {
          transmit("message", "rcvd continuous");
          gpsRequestMode = 2;
        } else if (datacsvarray[0] == "stop") {
          transmit("message", "rcvd gps stop");
          gpsRequestMode = 0;
        }
        targetTime = millis() + GPS_TIMEOUT;
      } else {
        //transmit("message", "datatype invalid");
        delay(100); // Do nothing
      }
    } else {
      //transmit("message", "empty data/dtype"); // Nothing on one side of colon character
      delay(100); // Do nothing
    }
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (gpsRequestMode == 0) { // gpsRequestMode will be 0 after waking up or if gps:stop is received
    requestSleep = true; // Request sleep
  } else {
    requestSleep = false; //Set requestSleep to false, just to be sure
    if (targetTime > millis() || GPS_TIMEOUT == 0) { // Only run if targetTime > millis() or GPS_TIMEOUT == 0
      bool newData = false;
      // For GPS_DATA_TRANSMIT_INTERVAL milliseconds we parse GPS data and report some key values
      for (unsigned long start = millis(); millis() - start < GPS_DATA_TRANSMIT_INTERVAL;) {
        while (ss.available()) {
          char c = ss.read();
          //Serial.write(c); // Uncomment this line if you want to see the GPS data flowing
          // Only uncomment if wireless communication module used is unplugged or can keep up with a lot of data
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

        flat   = flat               == TinyGPS::GPS_INVALID_F_ANGLE    ? 0.0    : flat;                              // Latitude
        flon   = flon               == TinyGPS::GPS_INVALID_F_ANGLE    ? 0.0    : flon;                              // Longitude
        sats   = gps.satellites()   == TinyGPS::GPS_INVALID_SATELLITES ? 0      : gps.satellites();                  // Satellites
        acc    = gps.hdop()         == TinyGPS::GPS_INVALID_HDOP       ? 0      : gps.hdop();                        // precision / accuracy
        spd    = gps.f_speed_kmph() == TinyGPS::GPS_INVALID_F_SPEED    ? 0      : gps.f_speed_kmph();                // speed
        dir    = gps.f_course()     == TinyGPS::GPS_INVALID_F_ANGLE    ? "*** " : TinyGPS::cardinal(gps.f_course()); // direction
        age    = age                == TinyGPS::GPS_INVALID_AGE        ? 0      : age;                               // age of data
        failed = failed             == 0xFFFFFFFF                      ? 255    : failed;                            // checksum

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

        if (gpsRequestMode != 2) { // if GPS request mode is not equal to 2. I.e. if gpsRequestMode is 1 (In all cases for this code). If statement is written like this so Arduino will sleep even if gpsRequestMode is set to an invalid number
          gpsRequestMode = 0; // Set gpsRequestMode to 0, which will loop around and then request sleep
        }
      }
    } else { // If Arduino can't get a GPS location after GPS_TIMEOUT ms ( targetTime < millis() )
      transmit("message", "Timeout no GPS"); // Tell the handheld we cannot get a GPS location
      gpsRequestMode = 0; // Stop requesting GPS data. Program will loop around and do requestSleep = true.
    }
  }
  if (millis() - startMillis > MAX_ON_TIME && MAX_ON_TIME != 0) { // Fail-safe if arduino is on for too long without contact from handheld controller
    transmit("message", "Failsafe sleep");
    gpsRequestMode = 0;
    requestSleep = true;
    // The program would ordinarily loop back around and then set requestSleep = true because gpsRequestMode == 0
    // However, setting it manually in this case to make sure the Arduino turns off after the MAX_ON_TIME to save power
  }
  if (requestSleep == true) {
    ArduinoSleep(); // Put the arduino into a light sleep, where it can still receive serial data on the UART port and store it in the serial buffer
  }
}
