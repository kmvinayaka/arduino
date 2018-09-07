#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

/////////*    Network Configuration     */////////////

#define WIFI_SSID        "XXXXXXXX"          //WiFi SSID name
#define WIFI_PASSWORD    "XXXXXXXX"         //WiFi Pasword

///////*     Access  Token From CLOUD   *////////////

#define TOKEN "XXXXXXXXXXXXX"                    //access Token ID from the Server
char cloudServer[] = "demo.thingsboard.io";    //Cloud Server address

///////*     DHT11 setup    ////////////

#include "DHT.h"
#define DHTPIN 5                         //Pin D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

unsigned long lastSend = 0;

void setup()
{
  Serial.begin(115200);                       // Initializing the Serial communication
  dht.begin();
  delay(10);
  InitWiFi();                                 // Setting up the network
  client.setServer( cloudServer, 1883 );      // Connecting with cloud Server
  lastSend = 0;
}

void loop()
{
  if ( !client.connected() )
  {
    reconnect();
  }

  if ( millis() - lastSend > 1000 )
  {
    temperature_humidity();                      //Function call
    lastSend = millis();
  }

  client.loop();
}

void temperature_humidity()                           //Function to collect the Temperature and Humidity
{
  Serial.println("Collecting temperature data.");
  float h =dht.readHumidity();                       // Reading humidity

  float t =dht.readTemperature();                    // Read temperature as Celsius

  if (isnan(h) || isnan(t))                           // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  String temperature = String(t);
  String humidity = String(h);
  /////*      Prepare a JSON payload string using Arduino JSON   *////////

  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["Sensor"] = "DHT11";
  JSONencoder["Temperature"] = temperature;
  JSONencoder["Humidity"] = humidity;
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);
  /////*      Send payload to the CLOUD telemetry *///////
  client.publish( "v1/devices/me/telemetry", JSONmessageBuffer);


}

void InitWiFi()                                       //Setup to connect through WiFi network
{
  Serial.println("Connecting to AP ...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect()                                      //Reconnecting with MQTT
{
  while (!client.connected())
  {

    if (  WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to CLOUD ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print(client.state());
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
