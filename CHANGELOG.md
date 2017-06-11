# arduino-gps-tracker changelog
## v0.1
* Initial release
* Handheld controller: Added the handheld controller under "gpsTracker_handheldController". It can display sent messages, received messages, latitude, longitude, number of satellites, accuracy, speed, direction, age of data and the checksum value.
* Handheld controller: Added map to TFT LCD screen. Map is not yet functional.
* Tracking device: Added the GPS tracker under "gpsTracker_trackingDevice". It can get GPS coordinates and transmit them when requested.
* Tracking device: Added code so the Arduino can go into a light sleep and wake up when serial data in the buffer. Sleep occurs after the Arduino has finished its task, couldn't receive coordinates before GPS_TIMEOUT milliseconds or, as a failsafe, the Arduino will sleep if it has been on for more than MAX_ON_TIME milliseconds as long as it hasn't received any messages from the handheld controller during this time.

## v0.2
* Handheld controller: Added ability to show the current location on the map
* Handheld controller: The program requires the max and min values of latitude and longitude covered by the map
* Handheld controller: Added 3 functions: clearTFTMAP(), drawTFTMap() and displayLocationOnMap(currentLatitude,currentLongitude)
* Handheld controller: Removed debugging code from gpsTracker_handheldController which printed to the UART Serial port
* Handheld controller: Renamed some functions and variables for clarity
* Tracking device: Renamed some functions and variables for clarity
* Tracking device: Tweaked delay values for better timing

## v0.2.1
* Handheld controller: Added clear map button on pin 8

## v0.3
* Handheld controller: Renamed LED_BUILTIN to LED_PIN and changed value to pin 9. Pin 13 is used by the TFT LCD screen
* Handheld controller: Changed the way location is drawn on the map (from 5 pixels in a '+' shape to a single pixel)
* Handheld controller: Added functionality to draw a green line to represent the tracker's location history
* Handheld controller test code: Added test code for drawing arrows on the screen
* Handheld controller: Added ability to draw an arrow at the edge of the screen if the tracker is outside the map
* Tracking device: Increased default MAX_ON_TIME to 1 hour

## v0.3.1
* Handheld controller: Added more constants under #define, for custom configurations

## v0.3.2
* Tracking device: Added KiCad circuit board files to "/gpsTracker_trackingDevice_circuitBoard/"

## v0.3.3
* Tracking device: Added more configuration constants
* Tracking device: Fixed bug where Arduino should stay awake to report continuous GPS data but would instead sleep if any message was received on the UART serial buffer
* Tracking device: Added more code comments

## v0.3.4
* Tracking device: Added ability for Arduino to control the GPS's power through pin 4. An N-channel MOSFET is required. See circuit diagram in gpsTracker_trackingDevice_circuitBoard/
* Tracking device: Updated OutputPowerSavingPins[] array to reflect changes

## v0.4 (Current release)
* Tracking device: Added readVcc() function which allows the Arduino to read its own Vcc input voltage. This will allow the user to know the charge state of the Li-ion cell(s)
* Tracking device: Tracking device will return cell charge in mV as: "cell:0000mV" when it receives "cell:volts"
* Tracking device: Removed "ping" message recognition code
* Handheld controller: Replaced ping button with cell voltage button
* Handheld controller: Removed "ping" message recognition code
* Handheld controller: Removed "age" and "check" areas from screen to make room for HCV:0000mV (Handheld controller voltage) and TDV:0000mV (Tracking device voltage)
* Handheld controller: Moved bottom-right values to the left by 1 character (6 pixels) to give more room for values
* Handheld controller: Modified clearTFTGPS(). Added clearTFT_HCV() and clearTFT_TDV()
* Handheld controller: Added code so the handheld controller can request and display the Vcc voltage of the GPS tracking device
* Handheld controller: Also added readVcc() function which allows the Arduino to read its own Vcc input voltage
* Handheld controller: Added code so the handheld controller reads its own Vcc input voltage every HCV_READVCC_INTERVAL in milliseconds and displays value on TFT screen. default HCV_READVCC_INTERVAL is 30000 ms
* Tracking device: Renamed GPS_DATA_TRANSMIT_FREQUENCY to GPS_DATA_TRANSMIT_INTERVAL
