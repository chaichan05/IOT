#include <Arduino.h>

int pin21Value;
const int PIN21 = 21;

void setup() {
// put your setup code here, to run once:
Serial.begin(115200);
pinMode(PIN21, INPUT_PULLUP);
}

void loop() {
pin21Value = digitalRead(PIN21);
Serial.print("PIN 21 = ");
Serial.println(pin21Value);
delay(50);
}