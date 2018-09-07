#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "DHT.h"
#define DHTPIN 5                         //Pin D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);                                  //Serial connection
  WiFi.begin("ssid", "password");   //WiFi connection

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion

    delay(500);
    Serial.println("Waiting for connection");

  }

}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("Collecting temperature data.");
    float h = dht.readHumidity();                       // Reading humidity

    float t = dht.readTemperature();
    /////*      Prepare a JSON payload string using Arduino JSON   *////////

    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
    JSONencoder["Sensor"] = "DHT11";
    JSONencoder["Temperature"] = t;
    JSONencoder["Humidity"] = h;
    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    /////--------------------------------------------------------------/////
    HTTPClient http;    //Declare object of class HTTPClient

    http.begin("http://demo.thingsboard.io/api/v1/1JJDUSf4iWah5nlKt7GI/telemetry");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header

    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload1 = http.getString();    //Get the response payload

    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload1);    //Print request response payload

    http.end();  //Close connection

  } else {

    Serial.println("Error in WiFi connection");

  }

  delay(1000);  //Send a request every 30 seconds

}
