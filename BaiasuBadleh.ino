#define samplingRate 10

void serialHandler()
{
  String serialString = Serial.readString();

  if (serialString == "1 A")
  {  
    PORTB |= 0x20;
  }

  else if (serialString == "1 S")
  {  
    PORTB &= ~0x20;
  }
  
  else if (serialString[0] == '2')
  {
    handleRGB(serialString);
  }
}


void handleRGB(String serialString)
{
  unsigned int rgbValues[6];
  unsigned int j = 0;
  for (auto i = 2; i <= 8; i++)
  {
    if (serialString[i] == 'A') rgbValues[j] = 10;
    else if (serialString[i] == 'B') rgbValues[j] = 11;
    else if (serialString[i] == 'C') rgbValues[j] = 12;
    else if (serialString[i] == 'D') rgbValues[j] = 13;
    else if (serialString[i] == 'E') rgbValues[j] = 14;
    else if (serialString[i] == 'F') rgbValues[j] = 15;
    else rgbValues[j] = serialString[i] - 48;
    j++;
  }
  
  byte red = rgbValues[0] * rgbValues[1];
  byte blue = rgbValues[2] * rgbValues[3];
  byte green = rgbValues[4] * rgbValues[5];
  
  analogWrite(11, red);
  analogWrite(10, blue);
  analogWrite(9, green);
}


void setup()
{
  DDRB = 0x2E; // Set B pins to output
  Serial.begin(9600); // BAUD 9600 bps
}

void loop()
{
  serialHandler(); // handles given serial data
 
  delay(samplingRate);
}
