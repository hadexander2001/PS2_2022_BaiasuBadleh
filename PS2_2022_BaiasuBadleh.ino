#include <EEPROM.h>

#define samplingRate 100

int count_message = 0;
float temp;
int address = 0;
int newaddress;

long duration;
int distance;

const int tempPin = 0;
const int trigPin = 9;
const int echoPin = 10;

void serialHandler()
{
  String serialString = Serial.readString();

  // Serial.print("In serial handler\n");
  
  if (serialString[0] == '1' && serialString[2] == 'A')
  {  
    PORTB |= 0x20;
  }

  else if (serialString[0] == '1' && serialString[2] == 'S')
  {  
    PORTB &= ~0x20;
  }
  
  else if (serialString[0] == '2')
  {
    handleRGB(serialString);
  }
  else if (serialString[0] == '6')
  {
    readMessages(serialString);
  }
}

void readTemperature()
{
  temp = analogRead(tempPin);

   // read analog volt from sensor and save to variable temp
  float temperature = ((temp * 5) / 1024 - 0.5) * 100;
   // convert the analog volt to its temperature equivalent
  //  Serial.print("3.< ");
   Serial.print(temperature); // display temperature value
   Serial.print(" ºC");
   Serial.println();
   delay(1000); // update sensor reading each one second
}

void readDistance()
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);

  distance = duration*0.034/2;

  if (distance <= 20 && distance > 10)
  {
    Serial.print("4.<:");
    Serial.print(distance);
    Serial.println(" cm - Risc de inundatie!!>");
    delay(1000);
  }
  else if (distance <= 10 && distance >= 1)
  {
    Serial.print("4.<:");
    Serial.print(distance);
    Serial.println(" cm - Risc RIDICAT de inundatie!!>");
    delay(1000);
  }
  if (distance>20)
  {
    Serial.print("4.<:");
    Serial.print(distance);
    Serial.println(" cm - Nu exista niciun risc de inundatie.>");
    delay(1000);
  }
}

void handleRGB(String serialString)
{
  // PORTD |= 0x68;
  Serial.flush();
  // Serial.print("In RGB handler\n");
  
  byte rgbValues[6];
  byte j = 0;
  
  for (auto i = 2; i <=  8; i++)
  {
    if (serialString[i]  == 'A') rgbValues[j] = 10;
    else if (serialString[i]  == 'B') rgbValues[j] = 11;
    else if (serialString[i]  == 'C') rgbValues[j] = 12;
    else if (serialString[i]  == 'D') rgbValues[j] = 13;
    else if (serialString[i]  == 'E') rgbValues[j] = 14;
    else if (serialString[i]  == 'F') rgbValues[j] = 15;
    else rgbValues[j] = serialString[i] - 48;
    j++;
  }
  
  byte red = rgbValues[0]*16 + rgbValues[1];
  Serial.print(red);
  Serial.print(" ");
  byte blue = rgbValues[2]*16 + rgbValues[3];
  Serial.print(blue );
  Serial.print(" ");
  byte green = rgbValues[4]*16 + rgbValues[5];
  Serial.print(green);
  Serial.print("\n");
  
  analogWrite(6, 255 - red);
  analogWrite(5, 255 - blue);
  analogWrite(3, 255 - green);
}

void writeEEPROM(int address, String str)
{
  byte len = str.length();
  EEPROM.write(address, len);

  for (int i = 0; i < len; i++)
  {
    EEPROM.write(address + 1 + i,str[i]);
  }
}

String readEEPROM(int address)
{
  int len = EEPROM.read(address);
  char data[len+1];

  for (int i = 0; i < len; i++)
  {
    data[i] = EEPROM.read(address+1+i);
  }
  data[len] = '\0';

  return String(data);
}

void readMessages(String serialString)
{
    String message;
  
   for (auto i = 2; i < serialString.length()-2; i++)
   {
    message.concat(serialString[i]);
    Serial.print(serialString[i]);
   }
   writeEEPROM(address, message);
   newaddress = address + 1 + message.length();
   address = newaddress;
 
}

void writeMessages()
{
  String str;
  int eepromOffset[10];
  int address = 0;
  byte len;
  int count = 1;
  
  while (count <= 10)
  {
    Serial.print(count);
    Serial.print(".");
    Serial.println(readEEPROM(address));
    len = readEEPROM(address).length();
    address = address + len + 1;
    count++;
  }
}

void setup()
{
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  DDRB = 0x22; // Set B pins 12 and 8 to output
  DDRD = 0x68; // Set PWM D pins to output

  PORTD |= 0x68;
  
  Serial.begin(9600); // BAUD 9600 bps

  // if (count_message == 0 && EEPROM.length() != 0)
  // {
  //   writeMessages();
  //   count_message++;
  // }
}

void loop()
{
  readTemperature();
  // readDistance();
  serialHandler(); // handles given serial data
  
  delay(samplingRate);
}
