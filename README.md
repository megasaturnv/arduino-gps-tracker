# arduino-gps-tracker
## Synopsis
An Arduino-based GPS tracking solution. One Arduino and Ublox Neo-6m (GY-NEO6MV2) GPS module is used as the GPS tracking device. The other is an Arduino handheld controller which requests information from the tracking device and displays the coordinates, GPS location and location on a map. By default, the display is a 128x160 1.8" TFT LCD but the code can be adapted for other displays. Communication occurs between the two devices with the HC-12 wireless serial communication module. This project can be adapted to use other methods of wireless communication such as the XBee or GSM mobile network. Please make sure the frequency used and transmitting power is legal in your country.

## Description of Arduino handheld controller
The Arduino handheld controller is used by a user to find the location of their GPS tracking device. It is powered by an 18650 Li-ion cell and consists of Li-ion protection and charging circuitry, an Arduino, a 1.8" TFT LCD display, a HC-12 wireless communication module and buttons for requesting various data from the tracking device. When it receives data from the tracking device, the Arduino will interpret the message formtted as \<datatype>:\<CSV of data> into \<data type> and an array of data for each item in \<CSV of data>. Then, the tracker respond in the appropriate way. The messages it understands and the way it responds are listed below:

#### if receive "volts:\[millivolts\]"
Display the tracking device Arduino's VCC voltage on the 1.8" TFT LCD display.

#### if receive "gps:\<latitude>, \<longitude>, \<num of satellites>, \<accuracy>, \<speed>, \<direction>, \<age of data>, \<checksum>"
Display these figures on the 1.8" TFT LCD display.

### Buttons
The buttons on the handheld controller are pulled high by the Arduino's internal pull-up resistors. To activate the button, the pin should be connected to ground. 

### The Arduino pins used to connect to peripherals is shown below:
#define name | Pin no. | Function
--- | --- | ---
SOFTWARE_SERIAL_TX | 2 | For the HC-12 module. Connects to HC-12 RX
SOFTWARE_SERIAL_RX | 3 | For the HC-12 module. Connects to HC-12 TX
CELLVOLTS_BUTTON | 4 | Push button connected to ground. transmit: "cell:volts" to the tracking device
REQUEST_GPS_ONCE_BUTTON | 5 | Push button connected to ground. transmit: "gps:once"
REQUEST_GPS_CONTINUOUS_BUTTON | 6 | Push button connected to ground. transmit: "gps:continuous"
REQUEST_GPS_STOP_BUTTON | 7 | Push button connected to ground. transmit: "gps:stop"
sclk | 13 | Connection to 1.8" TFT LCD screen. Connects to TFT SCK/
mosi | 11 | Connection to 1.8" TFT LCD screen. Connects to TFT SDA/
cs | 10 | Connection to 1.8" TFT LCD screen. Connects to TFT SCS/
dc | A0 | Connection to 1.8" TFT LCD screen. Connects to TFT A0/
rst | A1 | Connection to 1.8" TFT LCD screen. Connects to TFT RESET/

### Showing location on the display's map
*COMING SOON*

### How to create your own maps for the TFT LCD screen
*COMING SOON*

## Description of Arduino GPS tracking device
The Arduino tracking device should be attached to an object which the user wants to track. It is powered by an 18650 Li-ion cell and consists of Li-ion protection and charging circuitry, an Arduino, a Ublox Neo-6m (GY-NEO6MV2) GPS module and a HC-12 wireless communication module. When powered, the Arduino will go into a light sleep mode. It will wake up when it receives serial data from the HC-12 module. The Arduino will interpret the message formtted as \<datatype>:\<CSV of data> into \<data type> and an array of data for each item in \<CSV of data>. Then, the tracker respond in the appropriate way. The messages it understands and the way it responds are listed below:

#### if receive "cell:volts"
send "cell:\[millivolts\]" where millivolts = voltage of the tracking device Arduino's VCC pin

#### if receive "gps:once"
send "message:received gps once" and try to get GPS coordinates. Then, send coordinates once and go to sleep

#### if receive "gps:continuous"
send "message:received gps continuous" and try to get GPS coordinates. Then, send coordinates every 10 seconds

#### if receive "gps:stopcontinuous"
send "message:gpsstopcontinuous" and then stop sending GPS coordinates continuously

If the tracking device is requested for coordinates, it will try and get its GPS location. If it cannot get a fix after GPS_TIMEOUT milliseconds, the Arduino will send "message:Unable to get GPS fix" to the handheld controller, where it will be displayed on the screen. This functionality can be disabled by setting GPS_TIMEOUT to 0 (#define GPS_TIMEOUT 0).

If no more GPS coordinates are required (after a successful GPS lock with "gps:once" or "gps:stopcontinuous" commmand) or if the Arduino has been on for more than MAX_ON_TIME, go to sleep. MAX_ON_TIME is a failsafe to make sure the Arduino doesn't stay on for more than MAX_ON_TIME milliseconds. This functionality can be disabled by setting MAX_ON_TIME to 0 (#define MAX_ON_TIME 0).

### The Arduino pins used to connect to peripherals is shown below:

Pin description | Pin no. | Function
--- | --- | ---
SERIAL_UART_RX | 0 | For the HC-12 module. Connects to HC-12 TX
SERIAL_UART_TX | 1 | For the HC-12 module. Connects to HC-12 RX

#define name | Pin no. | Function
--- | --- | ---
SOFTWARE_SERIAL_RX | 3 | Software serial RX pin, to be connected to the GPS module's TX
SOFTWARE_SERIAL_TX | 2 | Software serial TX pin, to be connected to the GPS module's RX

## Motivation
I have created this project to be used as a dog tracker, but it can easily track a car and other valuable item.

## Installation
Upload gpsTracker_trackingDevice to the Arduino which will be used in the tracking device. Upload gpsTracker_handheldController to the Arduino which will be used in the handheld controller. Follow the build diagrams and create the circuit board from the KiCAD files to construct the project.

## Libraries used
### Handheld controller
* [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit_QDTech](https://github.com/zigwart/Adafruit_QDTech) Note my display is QDTech 1.8" 128x160 pixel LCD board using a Samsung S6D02A1 chip, so I need a special library.
* [SPI](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/SPI)
* [SoftwareSerial](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/SoftwareSerial)

### Tracking device
* [avr/power](https://github.com/Synapseware/avr/blob/master/include/avr/power.h)
* [avr/sleep](https://github.com/Synapseware/avr/blob/master/include/avr/sleep.h)
* [SoftwareSerial](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/SoftwareSerial)
* [TinyGPS](http://arduiniana.org/libraries/tinygps/)
