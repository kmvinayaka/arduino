#include <ESP8266WiFi.h>
#include <String.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
////////////////////////////////////////
#define WLAN_SSID       "XXXXXXXXXX"
#define WLAN_PASS       "XXXXXXXXXX"
//////////////////////////////////////////
#define AIO_SERVER      "io.adafruit.com" // server name
#define AIO_SERVERPORT  1883
//////////////////////////////////////////
#define IO_USERNAME  "XXXXXXXXXXXXXX" //user name from account
#define IO_KEY       "XXXXXXXXXXXXXXXXXXXXXX"// secret key
/////////////////////////////////////////
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);
////////////////////////////////////
Adafruit_MQTT_Publish LIGHT_Status = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/LIGHT_Status");
void MQTT_connect();
#define ldr A0
//////////////////////////////////////////
void setup()
{

  Serial.begin(9600);
  pinMode(ldr, INPUT);
  Serial.println(F("Adafruit MQTT demo"));
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  MQTT_connect();
  int x = analogRead(ldr);
  Serial.println(x);
  if (x > 800)
  {
    Serial.println("MORNING");
    LIGHT_Status.publish("MORNING");
  }
  else
  {
    Serial.println("NIGHT");
    LIGHT_Status.publish("NIGHT");
  }

  delay(1000);

}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
