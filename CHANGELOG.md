# arduino-gps-tracker changelog
## v0.1
* Initial release
* Added the handheld controller under "gpsTracker_handheldController". It can displays sent messages, received messages, latitude, longitude, number of satellites, accuracy, speed, direction, age of data and the checksum value.
* Added map to handheld controller. Map is not yet functional.
* Added the GPS tracker under "gpsTracker_trackingDevice". It can get GPS coordinates and transmit them when requested.
* Added code so the Arduino can go into a light sleep and wake up when serial data in the buffer. Sleep occurs after the Arduino has finished its task, couldn't receive coordinates before GPS_TIMEOUT milliseconds or, as a failsafe, the Arduino will sleep if it has been on for more than MAX_ON_TIME milliseconds as long as it hasn't received any messages from the handheld controller during this time.

## v0.2 (Current release - master branch)
* Added ability to show the current location on the map
* The program requires the max and min values of latitude and longitude covered by the map
* Added 3 functions: clearTFTMAP(), drawTFTMap() and displayLocationOnMap(currentLatitude,currentLongitude)
* Removed debugging code from gpsTracker_handheldController which printed to the UART Serial port
* Renamed some functions and variables for clarity
