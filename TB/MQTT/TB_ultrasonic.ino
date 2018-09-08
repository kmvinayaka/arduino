#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define WIFI_SSID "XXXXXXXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXXXX"

#define TOKEN "1JJDUSf4iWah5nlKt7GI"
char cloudServer[] = "demo.thingsboard.io";

#define TRIGGER 4 // defining trigger pin(D2 of nodemcu)
#define ECHO 5 // defining echo pin(D1 of Nodemcu)



WiFiClient wifiClient;

PubSubClient client(wifiClient);

unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  delay(10);
  InitWiFi();
  client.setServer( cloudServer, 1883 );
  lastSend = 0;
  pinMode(TRIGGER, OUTPUT); //initializing trigger as output
  pinMode(ECHO, INPUT); //initialing trigger as input
}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 )
  { // Update and send only after 1 seconds
    ultrasonic();
    lastSend = millis();
  }

  client.loop();
}

void ultrasonic()
{
  Serial.println("Collecting Distance data.");

  int duration, dist;
  digitalWrite(TRIGGER, LOW); // make trigger low
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH); // make trigger high
  delayMicroseconds(10); //give 10 microsec delay
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  dist = 34;//(duration / 2) / 29.1; // calibrate the distance using pulse
  Serial.print("distance : ");
  Serial.println(dist); // Print the distance value
  delay(500);
  
  String distance = String(dist);

  /////*      Prepare a JSON payload string using Arduino JSON   *////////

  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["Sensor"] = "HSR04";
  JSONencoder["Distance"] = distance;
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);
  /////*      Send payload to the CLOUD telemetry *///////
  client.publish( "v1/devices/me/telemetry", JSONmessageBuffer);

}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    if (  WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to cloud ...");
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
