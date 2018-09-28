#include <DHT.h>

#define DHTPIN 5                            //Pin D1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  Serial.println("Collecting temperature data.");
  float h = dht.readHumidity();                       // Reading humidity

  float t = dht.readTemperature();                    // Read temperature as Celsius

  if (isnan(h) || isnan(t))                           // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Temperature :");
  Serial.print(t);
  
  Serial.println("Humidity :");
  Serial.print(h);
  



  if(h>85)
   {
    digitalWrite(13,1);
   }
  else
  {
   digitalWrite(13,0);
  }

}

