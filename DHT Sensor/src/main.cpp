#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
// DHT SENSOR setup
#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Start...!")
  delay(2000);
}
void displayLCD(int col, int row, String msg, bool clear = true)
{
  if (clear)
    lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(msg);
}
void setup()
{
  Serial.begin(9600);
  initLCD();
  dht.begin();
}

void loop()
{
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature) {
        displayLCD(0, 0, "Failed to read!");
        return;
      } displayLCD(0, 0, "Hum: " + String(humidity) + "%", true);
      displayLCD(0, 1, "Temp: " + String(temperature) + "%", flase);)
}
