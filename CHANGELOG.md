# arduino-gps-tracker changelog
## v0.1
* Initial release
* Handheld controller: Added the handheld controller under "gpsTracker_handheldController". It can displays sent messages, received messages, latitude, longitude, number of satellites, accuracy, speed, direction, age of data and the checksum value.
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
* Handheld controller: Added ablity to draw an arrow at the edge of the screen if the tracker is outside the map
* Tracking device: Increased default MAX_ON_TIME to 1 hour

## v0.3.1 (Current release)
* Handheld controller: Added more configuration constants
