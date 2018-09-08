#include <ESP8266WiFi.h>
#include <Losant.h>
#include "DHT.h"
#define DHTPIN 5                         //Pin D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials.
const char* WIFI_SSID = "SSID";
const char* WIFI_PASS = "12345678q";

// Losant credentials.
const char* LOSANT_DEVICE_ID = "DEVICE ID";
const char* LOSANT_ACCESS_KEY = "ACCESS KEY";
const char* LOSANT_ACCESS_SECRET = "ACCESS SECRET";

// Create a secure WiFi client. This can also be an instance of the unsecured
// WiFiClient class, but the secure TLS client is recommended.
WiFiClientSecure wifiClient;

// Create an instance of a Losant device.
LosantDevice device(LOSANT_DEVICE_ID);

// Connects to WiFi and then to the Losant Platform.
void connect() {

  // Connect to WiFi.
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin("iPhone", "12345678q");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to Losant.
  Serial.println();
  Serial.print("Connecting to Losant...");

  // Connect the device instance to Losant using TLS encryption.
  device.connectSecure(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

  while (!device.connected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
}

void setup() {
  Serial.begin(115200);
 // dht.begin();
  while (!Serial) { }

  connect();
}
int lastUpdate = millis();
void loop() {

  // Every 15 seconds.
  if (millis() - lastUpdate > 15000) {

    // Read the analog input. Typically this will also involve some math
    Serial.println("Collecting temperature data.");
    float h = dht.readHumidity();                      // Reading humidity

    float t = dht.readTemperature();                   // Read temperature as Celsius

    if (isnan(h) || isnan(t))                           // Check if any reads failed and exit early (to try again).
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Build a JSON object with the state to report.
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& state = jsonBuffer.createObject();
    state["Temperature"] = t;
    state["Humidity"] = h;


    // Report the state to Losant.
    device.sendState(state);

    lastUpdate = millis();
  }
}

