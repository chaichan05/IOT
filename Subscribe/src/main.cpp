#include <Arduino.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

// LCD config
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// WIFI config
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WiFiClient wifiClient;

int count = 0;
int RED_PIN = 26;
int YELLOW_PIN = 25;
int BLUE_PIN = 32;

// MQTT client config
const char *mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
const char *topic_sub = "ITKPS/Labmqtt/IT";
PubSubClient mqttClient(wifiClient);

void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...!");
  delay(1000);
}
void displayLCD(int col, int row, String msg, bool clear = true)
{
  if (clear)
    lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(msg);
}

void setup_wifi()
{
  // Start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected on ");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(" MAC address: ");
  Serial.println(WiFi.macAddress());
}

void reconnectMQTT()
{
  // Loop until reconnected
  char mqttinfo[80];
  char clientId[100] = "\0";
  snprintf(mqttinfo, 75, "Attempting MQTT connection at %s:%d (%s/%s)...",
           mqttServer, mqttPort, mqttUser, mqttPassword);
  while (!mqttClient.connected())
  {
    Serial.println(mqttinfo);
    sprintf(clientId, "ESP32Client-%04X", random(0xffff));
    if (mqttClient.connect(clientId))
    {
      Serial.println("---> MQTT Broker connected...");
      mqttClient.subscribe(topic_sub); // subscribe here after connected
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Recommended: Wait 5 seconds before retrying
    }
  }
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  char Temp[128];
  char Humidity[128];
  bool behindspace = false;
  char payloadMsg[128];
  int tempIdx = 0, humIdx = 0;
  Serial.print("---> Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("     Message:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    payloadMsg[i] = (char)payload[i];
    if (payloadMsg[i] == ' ')
    {
      behindspace = true;
    }
    else if (!behindspace)
    {
      Temp[tempIdx++] = payload[i];
    }
    else
    {
      Humidity[humIdx++] = payload[i];
    }
  }
  payloadMsg[length] = '\0';
  Temp[tempIdx] = '\0';
  Humidity[humIdx] = '\0';
  Serial.println();
  Serial.println(Temp);
  Serial.println(Humidity);
  Serial.println("-----------------------");

  int temp = atoi(Temp);
  int hum = atoi(Humidity);

  if (temp > 35)
  {
    digitalWrite(RED_PIN, HIGH);
  }
  else
  {
    digitalWrite(RED_PIN, LOW);
  }

  // TOO COLD
  if (temp < 15)
  {
    digitalWrite(BLUE_PIN, HIGH);
  }
  else
  {
    digitalWrite(BLUE_PIN, LOW);
  }

  // HUMIDITY SQUAD
  if (hum < 30)
  {
    digitalWrite(YELLOW_PIN, HIGH);
  }
  else
  {
    digitalWrite(YELLOW_PIN, LOW);
  }
  displayLCD(0, 0, "Hum " + String(Humidity) + " %", true);
  displayLCD(0, 1, "Temp " + String(Temp) + " C", false);
}

void setup()
{
  Serial.begin(115200);
  initLCD();
  setup_wifi();
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  // setup MQTT
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback); // set the callback function
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }
  mqttClient.loop();
  delay(500);
}