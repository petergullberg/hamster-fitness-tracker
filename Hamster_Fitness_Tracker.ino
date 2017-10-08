/*
  Hamster Fitness Tracker

  Use a sensor to track the hamster fitness
  you can either use serial or LCD, pelase
  the LCD is a standard 20x4 LCD. It can quite easily be changed.
  As a sensor I use an inductor sensor I had as a spare, but any sensor that can trigger
  
  
  The circuit:
  - Using built in LED pin 13
  - Rotation sensor attached to pin 10
  - LCD connect as.
       LCD (4)  RS pin to digital pin 12
       LCD (6)  Enable pin to digital pin 11
       LCD (11) D4 pin to digital pin 5
       LCD (12) D5 pin to digital pin 4
       LCD (13) D6 pin to digital pin 3
       LCD (14) D7 pin to digital pin 1
       LCD (5)  R/W pin to ground
       LCD (1)  GND pin to ground
       LCD (2)  VCC pin to 5V
       LCD (3)  VO pin (pin 3) connected wtih a potentiometer for contrast

*/



// include the library code:
#include <stdarg.h>
#include <LiquidCrystal.h>

#define _LCD     1      // If _LCD =1 use LCD else use Serial.print

#if _LCD
// LCD paramters
// connected to:
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define PRINTF_BUF 80 // define the tmp buffer size (change if desired)
void LCD_printf(const char *format, ...)
{
  char buf[PRINTF_BUF];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  for (char *p = &buf[0]; *p; p++) // emulate cooked mode for newlines
  {
    if (*p == '\n')
      lcd.print('\r');
    lcd.print(*p);
  }
  va_end(ap);
}
#endif

#define DIAMETER 16.8         // Diameter of the wheel in cm
#define CNT_PER_RPM 2         // pegs per rotation?
#define CIRCUMFERENCE 52.77   // wheel_circumference 
// constants won't change. They're used here to set pin numbers:
const int sensorPin = 10;     // the number of the pushbutton pin

int sensorState = 0;         // variable for reading the pushbutton status
unsigned long currentTime;
unsigned long lastSecond;
unsigned long totalCnt;             // total count of triggers
int cnt;                    // current interval count
int lastState;

void setup() {
  // set up the LCD's number of columns and rows:
#if _LCD
  lcd.begin(20, 4);

#else   // else Serial
  Serial.begin(9600); // USB is always 12 Mbit/sec
  Serial.print("Logging start\n");
#endif

  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(sensorPin, INPUT);
  lastState = HIGH;     // To avoid false start...

  lastSecond = millis();

}

void loop() {
  // read the state of the sensor:
  sensorState = digitalRead(sensorPin);

 
  if (sensorState == HIGH) {
    if ( lastState == LOW ) {
      lastState = HIGH;   // Positive edge triggering
      cnt++;
    }
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    lastState = LOW;
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
  }
  currentTime = millis();
  if ( currentTime > (lastSecond + 1000) ) {
    float currentSpeed;
    totalCnt += cnt;     // add to total

    currentSpeed = (cnt / 2) * 52.16 * 3600 / 100;    // Convert to meter per hour

#if _LCD
    // print the number of seconds since reset:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  HAMSTER FITNESS!");

    lcd.setCursor(4, 1);
    if ( 0 == currentSpeed ) {
      lcd.print("Zzzzzzzzzzz");
    }
    else {
      lcd.print(currentSpeed / 1000.0);
      lcd.print(" km/h");
    }

    lcd.setCursor(0, 3);
    lcd.print( ((totalCnt / 2.0) * 52.16) / 100.0);
    lcd.print( " meter (totalt)");

#else
    Serial.print("\nCnts per second: ");
    Serial.print(cnt);
    Serial.print("\nCurrent Speed:");
    Serial.print(currentSpeed);
    Serial.print(" meter per hour");
    Serial.print("\nTotal revs.:");
    Serial.print(totalCnt / 2);
    Serial.print("\nTotal meters:");
    Serial.print(((totalCnt / 2.0) * 52.16) / 100.0);

    Serial.print("\n-------------");
    
#endif

    //reset the timer
    lastSecond = currentTime;
    cnt = 0;            // reset counter

  }


}
