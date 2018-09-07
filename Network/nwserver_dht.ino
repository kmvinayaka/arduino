#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTPIN 5     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321 
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "XXXXXXX";
const char* password = "XXXXXX";

// Web Server on port 80
WiFiServer server(80);


// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
  dht.begin();
}
// runs over and over again
void loop()
{
  // Listenning for new clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (c == '\n' && blank_line)
        {
          float temp, humi;
          humi = dht.readHumidity();
          String t = "yes";
          temp = dht.readTemperature();
          Serial.println(temp); Serial.println(humi);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // your actual web page that displays temperature and humidity
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head><body><h1>SENSOR DASHBOARD</h1><h3>Temperature in Celsius: ");
          client.println(temp);
          client.println("*C</h3><h3>Humidity: ");
          client.println(humi);
          client.println("%</h3><h3>");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
