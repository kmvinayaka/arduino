#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#define WIFI_AP "Connect!"//ssid
#define WIFI_PASSWORD "kmvgrp_1008" //password
#define TOKEN "0dUEnpCZKarVyJn6Nlhu" // access token
// DHT
#define DHTPIN 5
#define DHTTYPE DHT11
char demoServer[] = "demo.thingsboard.io";

WiFiClient wifiClient;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  client.setServer( demoServer, 1883 );
  lastSend = 0;
 }

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 500 ) { // Update and send only after 1 seconds
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }
  client.loop();
}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = 90;//dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = 34;//dht.readTemperature();
  String value = "false";
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  value = ( (h>100) ? ("true") : ("false"));
  String temperature = String(t);
  String Humidity = String(h);
   
  String payload = "{";
  payload += "\"value\":"; payload += value;payload += ",";
  payload += "\"Temperature\":"; payload += temperature;payload += ",";
  payload += "\"Humidity\":"; payload += Humidity;
  payload += "}";


  Serial.println(payload);
  char attributes[500];
  payload.toCharArray( attributes, 500 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );

}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
  
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
      
    }
    Serial.print("Connecting to demoserver node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}




