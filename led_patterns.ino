int ledPin[] = {6,5,4,3};

void setup()
{
  for (int i =0;i<4;i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop() 
{
  byte nums[] = {0, 1, 3, 6, 4, 12, 8, 12, 4, 6, 3, 1, 0};
  for (byte i = 0; i<13;i++)
  {
    displayBinary(nums[i]);
    delay(25);
  } 
}

void displayBinary(byte numToShow)
{
  for (int i =0;i<4;i++)
  {
    if (bitRead(numToShow, i)==1)
    {
      digitalWrite(ledPin[i], HIGH); 
    }
    else
    {
      digitalWrite(ledPin[i], LOW); 
    }
  }
}
