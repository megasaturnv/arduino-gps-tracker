#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

TinyGPS gps;
SoftwareSerial ss(4, 3);
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup()
{
  Serial.begin(115200);
  ss.begin(9600); //My GPS module has a baud rate of 9600
  lcd.begin(16, 2);
  lcd.print("test text");
  delay(1000);
  lcd.clear();

  pinMode(13, OUTPUT);

  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
}

void loop()
{
  digitalWrite(13, !digitalRead(13));

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (ss.available())
    {
      char c = ss.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  lcd.clear();

  if (newData) {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    lcd.setCursor(0, 0);
    lcd.print("LAT");
    lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    lcd.setCursor(0, 1);
    lcd.print("LON");
    lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    lcd.setCursor(12, 1);
    lcd.print("S=");
    lcd.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());

    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0) {
    Serial.println("** No characters received from GPS: check wiring **");
    lcd.setCursor(12, 0);
    lcd.print("****");
  } else {
    lcd.setCursor(12, 0);
    lcd.print("E=");
    lcd.print(failed);
  }
}

