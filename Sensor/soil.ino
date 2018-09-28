int soil = A0;
void setup()
{
  Serial.begin(9600);
  pinMode(soil, INPUT);
}
void loop()
{

  int avg_soil = analogRead(A0); 
  //Serial.print("Soil_analog=");
  Serial.println(avg_soil);
  delay(200);
}
