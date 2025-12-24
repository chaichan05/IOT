#include <Arduino.h>

const int LDR_PIN = 23;
const int RELAY_PIN = 22;

void setup()
{
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop()
{
  int ldrValue = digitalRead(LDR_PIN);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println(ldrValue);

  Serial.println(ldrValue);
  if (ldrValue == HIGH)
  {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relay ON - It's dark");
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay OFF - It's light");
  }
  delay(50);
}
