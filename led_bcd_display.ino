int ledPin[] = {16, 5, 4, 0};

void setup()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop()
{
  for (byte counter = 0; counter <= 15; counter++)
  {
    displayBinary(counter);
    delay(1000);
  }
}

void displayBinary(byte numToShow)
{
 /*the bitRead is used to read the 4 bit binary number
and then in for loop i. the i read the position of the bit 
and turn on the led ex: 1001 then the led connected at position
0 and 3 should be turn On rest OFF.*/
  for (int i = 0; i < 4; i++)
  {
    if (bitRead(numToShow, i) == 1)
    {
      digitalWrite(ledPin[i], HIGH);
    }
    else
    {
      digitalWrite(ledPin[i], LOW);
    }
  }

}
