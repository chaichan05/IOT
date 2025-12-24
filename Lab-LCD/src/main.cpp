#include <Arduino.h>
#include <liquidcrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int count = 0;
void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Strating...!");
  delay(2000);
}
void displayLCD(int col, int row, String msg) {
  lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(msg);
}

void setup() {
  Serial.begin(115200);
  initLCD();
}

void loop() {
  count++;
  displayLCD(7,0,String(count));
  delay(1000);
}
