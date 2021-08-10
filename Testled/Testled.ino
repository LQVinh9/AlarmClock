const int ledPin = 13;
int potPin=A2;
int ledState = LOW ;
unsigned long previousMillis = 0;
// will store last time LED was updated
int potValue = 0;
// interval at which to blink(millseconds)
void setup()
{
  pinMode(ledPin, OUTPUT);
}
void loop()
{
  potValue=analogRead(potPin);
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis>= potValue )
  {
    previousMillis = currentMillis;
    if(ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}
