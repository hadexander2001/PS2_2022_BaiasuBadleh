#include <EEPROM.h>
#include <LiquidCrystal.h>

#define samplingRate 100

int count_message = 0;
float temp;
float temperature;
int address = 0;
int newaddress;

long duration;
int distance;

const int tempPin = 0;
const int trigPin = 9;
const int echoPin = 10;

int start_state = 0;
int stateButton = 0;
int change = 0;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

String serialHandler(String serialString)
{
 
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
  else if (serialString[0] == 'b')
  {  
    return "back";
  }

  return serialString;
  
}

void readTemperature()
{
  temp = analogRead(tempPin);

   // read analog volt from sensor and save to variable temp
  temperature = ((temp * 5) / 1024 - 0.5) * 100;
   // convert the analog volt to its temperature equivalent

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
    Serial.print(distance);
    Serial.println(" cm - Flooding danger");
    delay(1000);
  }
  else if (distance <= 10 && distance >= 1)
  {
    Serial.print(distance);
    Serial.println(" cm - High risk of flooding");
    delay(1000);
  }
  if (distance > 20)
  {
    Serial.print(distance);
    Serial.println(" cm - No flooding risk");
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
  // Serial.print(red);
  // Serial.print(" ");
  // byte blue = rgbValues[2]*16 + rgbValues[3];
  // Serial.print(blue );
  // Serial.print(" ");
  // byte green = rgbValues[4]*16 + rgbValues[5];
  // Serial.print(green);
  // Serial.print("\n");
  
  analogWrite(11, 255 - red);
  // analogWrite(5, 255 - blue);
  // analogWrite(3, 255 - green);
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

enum state {
  EV_OK,
  EV_CANCEL,
  EV_NEXT,
  EV_PREV,
  EV_NONE,
  EV_MAX_NUM
};

enum Menus {
  MENU_MAIN = 0,
  MENU_MESAJE,
  MENU_CONTROL,
  MENU_TEMPERATURA,
  MENU_INUNDATII,
  MENU_MAX_NUM
 
};


Menus scroll_menu = MENU_MAIN;
Menus current_menu =  MENU_MAIN;

void state_machine(enum Menus menu, enum state event);
state getState(String option);

void print_menu(enum Menus menu);

typedef void (state_machine_handler_t)(void);

void print_menu(enum Menus menu)
{
  lcd.clear();
  switch(menu)
  {
    case MENU_MESAJE:
    lcd.setCursor(0, 1);
    lcd.print("1. MESAJE");
    break;
   
    case MENU_CONTROL:
    lcd.setCursor(0, 1);
    lcd.print("2. CONTROL");
    break;
    
    case MENU_TEMPERATURA:
    lcd.setCursor(0, 1);
    lcd.print("3. TEMPERATURA");
    break;
    
    case MENU_INUNDATII:
    lcd.setCursor(0, 1);
    lcd.print("4. INUNDATII");
    break;
    
    case MENU_MAIN:
    default:
      lcd.setCursor(0,0);
      lcd.print("PS 2022");
      lcd.setCursor(0,1);
      lcd.print("BaiasuBadleh");
  }
}     

void enter_menu(void)
{
  current_menu = scroll_menu;
}

void go_home(void)
{
  scroll_menu = MENU_MAIN;
  current_menu = scroll_menu;
  change = 0;
}

void message(void)
{
  char option;
  String serialString;
  int i = 0;
  
  switch(scroll_menu)
  {
  case 1:
    
  lcd.clear();
  do
  {
    option=Serial.read();
    if(option=='u')
    {
      i++;
    }
    if(option=='d')
    {
      i--;
    }
    if(i==4)
    {
      i=0;
    }
    if(i==-1)
    {
      i=3;
    }
    
    switch(i)
    {
      case 0:lcd.setCursor(0,0);
             lcd.print("1.1 Citite  ");
             if(option=='k')
                {
                  writeMessages();
                }
      break;
      
      case 1:lcd.setCursor(0,0);
             lcd.print("1.2 Necitite");
      break;
      
      case 2:lcd.setCursor(0,0);
            lcd.print("1.3 Stergere");
      break;
    }
   
   }while(option!='b');
    break;

    case 2:
    
     lcd.clear();
    
    do{
      
    //option=Serial.read();
    serialString = Serial.readString();
    
    if(serialString[0]=='u')
    {
      i++;
    }
    if(serialString[0]=='d')
    {
      i--;
    }
    if(i==4)
    {
      i=0;
    }
    if(i==-1)
    {
      i=3;
    }
    
    switch(i)
    {
      case 0:lcd.setCursor(0,0);
             lcd.print("1.1 LED Control");
             
              if (serialString[0] == '1' && serialString[2]== 'A')
              {  
                PORTB |= 0x20;
              }

              else if (serialString[0] == '1' && serialString[2]== 'S')
              {  
              PORTB &= ~0x20;
              }
          
          
            break;
      
      case 1:lcd.setCursor(0,0);
             lcd.print("1.2 RGB Led Control");
             if (serialString[0] == '2')
             {
               handleRGB(serialString);
             }
      break;
      
      case 2:lcd.setCursor(0,0);
           
            lcd.print("1.3 Scriere Mesaj");
            if (serialString[0]=='6')
            {
             readMessages(serialString);
            }
            break;
       }
    
     }while(serialString[0]!='b');
     
    break;
    
    case 3:
    
     lcd.clear();
     
     do
    {
      readTemperature();
      lcd.print(temperature);
      lcd.print("*C");
      delay(1000);
      option=Serial.read();
      lcd.clear();
       
    } while(option != 'b');

    break;
    
    
    case 4:
  
  //lcd.clear();
  //lcd.print("inundatii");
  do
  {
    readDistance();
    option=Serial.read();
    
  }while(option!='b');
   break;
  
    
  }
}


void go_next(void)
{
  scroll_menu = (Menus) ((int)scroll_menu + 1);
  scroll_menu = (Menus) ((int)scroll_menu % MENU_MAX_NUM);
}

void go_prev(void)
{
  scroll_menu = (Menus) ((int)scroll_menu - 1);
  scroll_menu = (Menus) ((int)scroll_menu % MENU_MAX_NUM);
}


state_machine_handler_t* sm[MENU_MAX_NUM][EV_MAX_NUM] = 
{ //events: OK , CANCEL , NEXT, PREV
  {enter_menu, go_home, go_next, go_prev}, // MENU_MAIN
  {message,go_home,go_next,go_prev}, //MESAJE
  {message,go_home,go_next,go_prev}, //CONTROL
  {message,go_home,go_next,go_prev}, //TEMPERATURA
  {message,go_home,go_next,go_prev}, //INUNDATII
};
  
void state_machine(enum Menus menu, enum state event)
{
  sm[menu][event]();
}

state getState(String option)
{
  
  enum state ret_val = EV_NONE;
  if (option[0] == 'k')
  {
    ret_val = EV_OK;
  }
   if (option[0] == 'b')
  {
    ret_val = EV_CANCEL;
  }
   if (option[0] == 'u')
  {
    ret_val = EV_NEXT;
  }
   if (option[0] == 'd')
  {
    ret_val = EV_PREV;
  }
  return ret_val;
}

void setup()
{
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  DDRB = 0x2A; // Set B pins 12 11 9 to output

  PORTB |= 0x08;
  
  Serial.begin(9600); // BAUD 9600 bps
  lcd.begin(16,2);

}

void loop()
{
  String serialString = Serial.readString();
  
  // readTemperature();
  readDistance();
  serialHandler(serialString); // handles given serial data

  
  volatile state event = getState(serialString);
  if (event != EV_NONE)
  {
      state_machine(current_menu, event);
  }
  print_menu(scroll_menu);
  
  delay(samplingRate);
}
