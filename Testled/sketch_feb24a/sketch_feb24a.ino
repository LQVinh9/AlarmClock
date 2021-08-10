int temValue;
int tempPin=1;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  temValue=analogRead(tempPin);
  float tempCelsius=(temValue*5.0/1024.0)*100.0;
  float TempFahrenheit=(tempCelsius*9)/5+32;
  Serial.print("temperature in Celsius is: ");
  Serial.println(tempCelsius);
  delay(1000);
}
