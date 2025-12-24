#include <Arduino.h>

const int BUTTON_PIN = 23;
const int RELAY_PIN = 22;
void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop()
{
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == 0)
  {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relay Activate ");
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay Deativate ");
  }
  delay(50);
}
