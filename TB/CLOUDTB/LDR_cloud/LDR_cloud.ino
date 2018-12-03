#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "XXXXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXX"

#define TOKEN "XXXXXXXXXXXX"

char cloudServer[] = "XXXXXXX";

WiFiClientSecure wifiClient;

int LDR = A0;

PubSubClient client(wifiClient);

unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  delay(10);
  InitWiFi();
  client.setServer(cloudServer, 1883 );
  lastSend = 0;
  pinMode(LDR, INPUT);
}

void loop()
{
  if ( !client.connected() )
  {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    soilmoist();
    lastSend = millis();
  }

  client.loop();
}

void soilmoist()
{
  Serial.println("Collecting Soil moisture data.");

  int data = 500;
  //int data = analogRead(LDR);
  Serial.print("Light Intensity: ");
  Serial.println(data);

  String Status ;

  if (data > 80 && data < 1200)
  {
    Status = " LIGHT";
  }
  else
  {
    Status = "DARK";
  }
  data = 10;
  String lddr = String(data);

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"Light Intensity\":";
  payload += lddr;
  payload += ",";
  payload += "\"Light Status\":";
  payload += Status;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );

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
