#include <Arduino.h>

int pin21value;
const int PIN21 = 21;
const int PIN12 = 12;
const int PIN19 = 19;

void setup() {
    Serial.begin(11520);
    pinMode(PIN21, INPUT_PULLUP); //input_pullup คือการตั้งค่าให้ขาเป็นอินพุตและมีการดึงขึ้นด้วยตัวต้านทานภายใน
    pinMode(PIN12, OUTPUT);
    pinMode(PIN19, OUTPUT);
}

void loop() {
    pin21value = digitalRead(PIN21);
    // Serial.print("PIN 21 = ");
    // Serial.println(pin21value);

    if (pin21value == LOW)
    {
        digitalWrite(PIN12, HIGH); //digitalWrite คือการตั้งค่าสถานะของขา GPIO
        digitalWrite(PIN19, LOW);

    }
    else {
        digitalWrite(PIN12,LOW);
        digitalWrite(PIN19,HIGH);
    }
    delay(100);
}