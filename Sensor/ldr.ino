void setup() 
{
  Serial.begin(9600);
}
void loop() 
{
  int sensorValue = analogRead(5);   // read the input on analog pin 0:
  Serial.print(sensorValue);        // display the values of 
  float voltage = sensorValue * (5 / 4096);   // Convert the analog reading to a voltage
 Serial.print("    ");
  // print out the value you read:
  Serial.println(voltage);
  delay(1000);
  if(sensorValue==100)
  {
    digitalWrite(5,HIGH);
    delay(3000);
  }
}
