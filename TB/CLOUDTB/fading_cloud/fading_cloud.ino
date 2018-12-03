#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "XXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXX"

#define TOKEN "XXXXXXXXXXXXXX"
char cloudServer[] = "XXXXXXXXXXXXXXX";

#define GPIO5 5
#define GPIO5_PIN 5
String status1;


WiFiClient wifiClient;

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;

// We assume that all GPIOs are LOW
//boolean gpioState[5];
boolean gpioState[5] = {false};

void setup() {
  Serial.begin(115200);
  // Set output mode for all GPIO pins

  pinMode(GPIO5, OUTPUT);
  digitalWrite(GPIO5, LOW);
  delay(10);
  InitWiFi();
  client.setServer( cloudServer, 1883 );
  client.setCallback(on_message);
}

void loop() {
  if ( !client.connected() ) {
    reconnect();
  }

  client.loop();
}

// The callback for when a PUBLISH message is received from the server.
void on_message(const char* topic, byte* payload, unsigned int length) {

  Serial.println("On message");

  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(json);

  // Decode JSON request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);

  if (!data.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  String methodName = String((const char*)data["method"]);

  if (methodName.equals("getValue")) {
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
  } else if (methodName.equals("setValue")) {
    set_gpio_status(data["params"]);
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    }  
}

String get_gpio_status() {
  status1 = gpioState[5] ? true : false;
  Serial.print("Get gpio status: ");
  Serial.println(status1);
  return status1;
}
void  set_gpio_status(boolean statu) {
    analogWrite(GPIO5, statu );
    gpioState[5] = statu;  
}


void InitWiFi() {
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
    Serial.print("Connecting to cloud node ...");
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );  
      client.subscribe("v1/devices/me/rpc/request/+");
      Serial.println("Sending RPS Request ...");
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 5000 );
    }
  }
}

