#include <Arduino.h>
#define RED_LED 23
#define YELLOW_LED 5

const int RED_DELAY = 100;
const int YELLOW_DELAY = 1000;
unsigned long previousTimeRed = 0;
unsigned long previousTimeYellow = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
}

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - previousTimeRed > RED_DELAY)
  {
    digitalWrite(RED_LED, HIGH);
    previousTimeRed = currentTime;
  }
  else
  {
    digitalWrite(RED_LED, LOW);
  }
  if (currentTime - previousTimeYellow > YELLOW_DELAY)
  {
    digitalWrite(YELLOW_LED, HIGH);
    previousTimeYellow = currentTime;
  }
  else
  {
    digitalWrite(YELLOW_LED, LOW);
  }
}
