#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

/* ===================== WIFI ===================== */
const char *ssid = "Wokwi-GUEST";
const char *password = "";

/* ===================== MQTT ===================== */
const char *mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char *topic_pub = "ITKPS/Labmqtt/IT";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

/* ===================== LCD ===================== */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* ===================== DHT22 ===================== */
#define DHTPIN 12        // เปลี่ยนเป็น 14 ถ้าใช้บอร์ดจริง
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/* ===================== LED ===================== */
#define RED_PIN     25
#define YELLOW_PIN  26
#define BLUE_PIN    32

/* ===================== FUNCTIONS ===================== */
void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(1500);
}

void displayLCD(int col, int row, String msg, bool clear = true)
{
  if (clear) lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(msg);
}

void setup_wifi()
{
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Connecting MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str()))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      delay(3000);
    }
  }
}

/* ===================== SETUP ===================== */
void setup()
{
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  dht.begin();
  initLCD();

  setup_wifi();
  mqttClient.setServer(mqttServer, mqttPort);
}

/* ===================== LOOP ===================== */
void loop()
{
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }
  mqttClient.loop();

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp))
  {
    displayLCD(0, 0, "DHT Error!");
    delay(2000);
    return;
  }

  /* ---------- LCD ---------- */
  displayLCD(0, 0, "Hum: " + String(hum) + "%");
  displayLCD(0, 1, "Temp: " + String(temp) + "C", false);

  /* ---------- LED Logic ---------- */
  // Temperature
  if (temp >= 35)
  {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  }
  else if (temp <= 15)
  {
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
  }
  else
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }

  // Humidity
  digitalWrite(YELLOW_PIN, hum < 30 ? HIGH : LOW);

  /* ---------- MQTT Publish ---------- */
  String payload = "{\"temp\":" + String(temp) +
                   ",\"hum\":" + String(hum) + "}";

  mqttClient.publish(topic_pub, payload.c_str());
  Serial.println(payload);

  delay(2000);
}
