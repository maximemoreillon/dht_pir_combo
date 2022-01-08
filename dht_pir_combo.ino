/*
 * DHT PIR COMBO
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */


// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <PubSubClient.h>
#include <DHT.h> // Temperature and humidity sensor
#include <ArduinoJson.h>

#include "credentials.h";

// OTA
#define HOSTNAME "livingMotionDHT"

// MQTT
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_MOTION_STATUS_TOPIC "motion/living/status"
#define MQTT_DHT_STATUS_TOPIC "dht/living/status"
#define MQTT_LAST_WILL "{'state':'disconnected'}"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// Pin mapping
#define DHT_PIN D1
#define PIR_PIN D2

// DHT
#define DHT_PUBLISH_PERIOD 300000 // [ms] = 5 minutes
#define DHT_READ_PERIOD 10000 // [ms] = 10 seconds

// Global variables
WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);

DHT dht(DHT_PIN, DHT22);

void setup() {

  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));

  // PIR init
  pinMode(PIR_PIN, INPUT);

  dht.begin();
  wifi_setup();
  MQTT_setup();
  OTA_setup();
}

void loop() {
  ArduinoOTA.handle();
  MQTT_client.loop();
  read_PIR();
  read_DHT();
  wifi_connection_manager();
  MQTT_connection_manager();
}
