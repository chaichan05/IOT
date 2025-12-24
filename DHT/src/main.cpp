#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

#define RED_PIN 25
#define YELLOW_PIN 26
#define BLUE_PIN 32
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
  lcd.print("Start...!");
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
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  initLCD();
  dht.begin();
}

void loop()
{
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    displayLCD(0, 0, "Failed to read!");
    return;
  }

  displayLCD(0, 0, "Hum: " + String(humidity) + "%", true);
  displayLCD(0, 1, "Temp: " + String(temperature) + "C", false);

  if (temperature >= 35)
  {
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
  }
  else if (temperature <= 15)
  {
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
  }else {
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
  }

  if (humidity < 30)
  {
    digitalWrite(YELLOW_PIN, HIGH);
  }
  else
  {
    digitalWrite(YELLOW_PIN, LOW);
  }

  delay(1000);
}
