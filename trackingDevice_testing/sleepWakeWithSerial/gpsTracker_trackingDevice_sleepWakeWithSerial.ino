/*Sleep Demo Serial
  Based on tutorial code from http://rubenlaguna.com/wp/2008/10/15/arduino-sleep-mode-waking-up-when-receiving-data-on-the-usart/

  Example code to demonstrate the sleep functions in a Arduino.
  Arduino will wake up when new data is received in the serial port USART.
*/

#include <avr/power.h>
#include <avr/sleep.h>

byte count = 0;                   // counter

const byte OutputPowerSavingPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}; // Unused pins that should be set as outputs (to save power)

void setup() {
  Serial.begin(9600);

  //Save Power by writing all Digital IO to outputs. Remember to not damage any attached devices. Max pin current ~ 20mA

  //Loop for the amount of values in OutputPowerSavingPins array.
  //In this case, sizeof(byte) == 1 so it's not really needed.
  //This is better practice when needing to know the amount of items in an int array.
  //for (byte i = 0; i < (sizeof(OutputPowerSavingPins) / sizeof(byte)); i++) {
  //  pinMode(OutputPowerSavingPins[i], OUTPUT);
  //  Serial.print(String(OutputPowerSavingPins[i]) + " ");
  //}
  Serial.println();
}

void sleepNow() {
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
}

void loop() {
  // Display information about the counter
  Serial.print("Awake for ");
  Serial.print(count);
  Serial.println("sec");
  count++;
  delay(1000); // Waits for a second

  // Compute the serial input
  if (Serial.available()) {
    int val = Serial.read();
    Serial.println("read: " + String(val));
    if (val == 'S') {
      Serial.println("Serial: Entering Sleep mode");
      delay(100);     // This delay is needed, the sleep function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // Sleep function called here
    }
    if (val == 'A') {
      Serial.println("Received letter A"); // Dummy message
    }
  }

  // Check if it should go asleep because of time
  if (count >= 10) {
    Serial.println("Timer: Entering Sleep mode");
    delay(100);     // This delay is needed, the sleep function will provoke a Serial error otherwise!!
    count = 0;
    sleepNow();     // Sleep function called here
  }
}

