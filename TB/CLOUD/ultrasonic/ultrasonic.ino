#include <ESP8266WiFi.h>
#include <String.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

////////////////////////////////////////
#define trigPin 5
#define echoPin 4
#define WLAN_SSID       "XXXXXXXXXXXX"
#define WLAN_PASS       "XXXXXXXXXXXX"
//////////////////////////////////////////
#define AIO_SERVER      "io.adafruit.com" // server name
#define AIO_SERVERPORT  1883
//////////////////////////////////////////
#define IO_USERNAME  "XXXXXXXXXXXXXX" //user name from account
#define IO_KEY       "XXXXXXXXXXXXXXXX"// secret key
/////////////////////////////////////////
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);
////////////////////////////////////
Adafruit_MQTT_Publish DISTANCE = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/DISTANCE");
void MQTT_connect();
//////////////////////////////////////////
void setup()
{

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  uint32_t duration, distance;
  digitalWrite(trigPin, LOW); // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 35.3;
  Serial.println(distance);
  DISTANCE.publish(distance);
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
