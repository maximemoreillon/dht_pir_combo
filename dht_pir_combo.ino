// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <AsyncMqttClient.h> // MQTT library
#include <Ticker.h> // Used when reconecting MQTT upon wifi drop
#include <DHT.h> // Temperature and humidity sensor
#include <ArduinoJson.h>

#include "credentials.h";

// Wifi
WiFiEventHandler wifi_connect_handler;
WiFiEventHandler wifi_disconnect_handler;
Ticker wifi_reconnect_timer;

// OTA
#define HOSTNAME "bedroomMotionDHT"

// MQTT
AsyncMqttClient MQTT_client;
Ticker MQTT_reconnect_timer;
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_MOTION_STATUS_TOPIC "motion/bedroom/status"
#define MQTT_DHT_STATUS_TOPIC "dht/bedroom/status"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// Pin mapping
#define DHT_PIN D1
#define PIR_PIN D2

// DHT variables
DHT dht(DHT_PIN, DHT22);
#define DHT_PUBLISH_PERIOD 300000 // [ms], 300000 is 5 minutes
#define DHT_READ_PERIOD 10000 // [ms]

long last_DHT_publish_time, last_DHT_read_time;
float DHT_temperature = 0;
float DHT_humidity = 0;

// PIR variables
int last_PIR_reading;

void setup() {

  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));

  // PIR init
  pinMode(PIR_PIN, INPUT);

  // DHT init
  dht.begin();

  wifi_setup();
  MQTT_setup();
  OTA_setup();

  wifi_connect();
}

void loop() {
  ArduinoOTA.handle();
  read_PIR();
  read_DHT();
}
