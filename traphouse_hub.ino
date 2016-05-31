/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <SoftwareSerial.h>
#include <SmartCityNodeLib.h>
#include "Adafruit_LiquidCrystal.h"
#include "Wire.h"

Adafruit_LiquidCrystal lcd(0);

long runTime;

// some constants to identify my node to the library object
const String myNodeName = "1017 Trap House";
const int    myZone = SmartCityNode::RESIDENTIAL;  // declare what zone you are

SmartCityNode myNode;
boolean myDebugging = true;

Servo myServo1;  // create servo object to control drawbridge servo
Servo myServo2; // servo object that controls panic room door

int posServo1 = 0;    // variable to store the servo position
int posServo2 = 90; // variable for servo controlling panic room door

const int hallPin = 12; // Hall effect sensor
const int moatButtonPin = 4; // Button to control moat drawbridge
const int ledPin = 13;
const int trapDoorBtn = 8; // panic room button
const int magneticSwitchPin = 5; // magnetic door sensor pin

const int analogInPin = A0; // analog input pi tat potentiometer is attached to
const int analogOutPin = 9; // analog out pin that the LED is attached to

int lcdPotValue = 0;
int lcdOutputValue = 0;
int hallState = 0;
int moatButtonState = 0;
int panicButtonState = LOW; // panic room button state
int magneticDirectionState = 0;
int ledState = LOW;

void setup() {
  myServo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myServo2.attach(10); // attaches panic room servo on pin 10
  Serial.begin(9600);
  pinMode(hallPin, INPUT);
  pinMode(moatButtonPin, INPUT);
  pinMode(7, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trapDoorBtn, INPUT);
  pinMode(magneticSwitchPin, INPUT);
  digitalWrite(magneticSwitchPin, HIGH);
  digitalWrite(ledPin, LOW);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("1017");
  lcd.setCursor(0, 1);
  lcd.print("Trap House");
  lcd.setBacklight(HIGH);

  Serial.println("\n\n*** Starting SmartCityNode demo");

  // you want to blink that LED, right?
  pinMode(13, OUTPUT);

  // just flash it for a second at startup so we know things have initialized
  digitalWrite(13, HIGH);
  delay(1000); 
  digitalWrite(13, LOW);

  // comment this line out to suppress debugging messages from the library
  myNode.setDebugOn();

  // this starts up your XBee and does other SmartCity initialization stuff.
  // the second argument is just to identify your node by a name.
  // you must declare what zone your node is in so that you only receive alerts
  // targeted for that zone. to do that, use one of these codes for the first
  // argument:
  //       SmartCityNode::RESIDENTIAL
  //       SmartCityNode::BUSINESS
  //       SmartCityNode::INDUSTRIAL

  myNode.begin(myZone, myNodeName);

}  


void loop() {
  lcd.setCursor(0, 0);
  lcd.setBacklight(HIGH);

  hallState = digitalRead(hallPin);
  moatButtonState = digitalRead(moatButtonPin);
  panicButtonState = digitalRead(trapDoorBtn);


  // Alert responses
  int alert = myNode.alertReceived();
  if (alert != 0) {
    // in this demo, we'll just print out a message about it
    logAlert(myNodeName, myZone, alert);
    // here is where you would do the real work of processing this alert:
    if (alert == SmartCityNode::BLACK_FRIDAY || alert == SmartCityNode::FIRE) {
      digitalWrite(13, HIGH);
      delay(2000);
      digitalWrite(13, LOW);
    } else {
      digitalWrite(13, LOW);
    }
    if (alert == SmartCityNode::FLOOD) {
      lcd.setCursor(0, 1);
      lcd.print("Flood Detected!");
      openDrawBridge();
    }
    if (alert == (SmartCityNode::BURGLARY || SmartCityNode::ZOMBIE || SmartCityNode::HIPSTER_INVASION)) {
      lcd.setCursor(0, 1);
      lcd.print(alert + " Detected!");
      openPanicRoom();
    }
    if (alert == SmartCityNode::FIRE) {
      lcd.setCursor(0, 1);
      lcd.print("Fire Detected!");
      // flash LEDs plus warning on lcd
    }
  }
  // End alert responses

  myServo2.write(posServo2);

// Servo Drawbridge controlled by button and hall effect sensor
  if (hallState == LOW || moatButtonState == HIGH) {
   // flashLED();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Opening Drawbridge");
    Serial.println(hallState);
    Serial.println("Magnet detected");
    Serial.println("Rotation started");
    myServo1.write(0);
    delay(5000);
    for (posServo1 = 0; posServo1 <= 90; posServo1 += 1) { // goes from closed degrees to open
      // in steps of 1 degree
      myServo1.write(posServo1);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(5000);
  }
  delay(5);
// End drawbridge code

// servo panic room door controlled by button
  if (panicButtonState == HIGH & posServo2==0) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Opening Panic Room");
      for (posServo2=0; posServo2 <= 89; posServo2=posServo2+1); {
        Serial.println(posServo2);
        myServo2.write(posServo2); 
        Serial.print("wrote");
        Serial.println(posServo2);
        delay(50);  
        panicButtonState =LOW;                       
      }
  }
  if (panicButtonState == HIGH & posServo2==90) {
      for (posServo2=90; posServo2>=1; posServo2=posServo2-1); {
        Serial.print("2loop");
        Serial.println(posServo2);
        myServo2.write(posServo2); 
        Serial.print("here");
        Serial.println(posServo2);
        delay(15); 
        panicButtonState = LOW;         
     }
  }
// end panic room button code

// Magnetic door sensor 
  if(digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 0) {
    ledState = HIGH;
    digitalWrite(ledPin, HIGH);
    flashLED();
  }
  if (digitalRead(magneticSwitchPin) == LOW & ledState == HIGH) {
    magneticDirectionState = 1;
  }
  if(digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 1) {
    ledState = LOW;
    digitalWrite(ledPin, LOW);
  }
  if (digitalRead(magneticSwitchPin) == LOW & ledState == LOW) {
    magneticDirectionState = 0;
  }
// End magnetic door sensor 

}
void openDrawBridge() { // Opens then closes drawbridge
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Opening Drawbridge");
  Serial.println("Magnet detected");
  Serial.println("Rotation started");
  myServo1.write(0);
  delay(5000);
  for (posServo1 = 0; posServo1 <= 90; posServo1 += 1) { // goes from closed degrees to open
    // in steps of 1 degree
    myServo1.write(posServo1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(5000);
}

void openPanicRoom() { // Opens then closes panic room door
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Opening Panic Room");
  for (posServo2=0; posServo2 <= 89; posServo2=posServo2+1); {
    Serial.println(posServo2);
    myServo2.write(posServo2); 
    Serial.print("wrote");
    Serial.println(posServo2);
    delay(15);  
    panicButtonState =LOW;                       
   }
  for (posServo2=90; posServo2>=1; posServo2=posServo2-1); {
    Serial.print("2loop");
    Serial.println(posServo2);
    myServo2.write(posServo2); 
    Serial.print("here");
    Serial.println(posServo2);
    delay(15); 
    panicButtonState = LOW;         
  }  
}

void flashLED() {
  Serial.println("FLASHING LEDS!");
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("Flashing LEDs");
  int timer = millis();
  digitalWrite(7, HIGH);
  if (millis() == (timer + 250)) {
    digitalWrite(7, LOW);
  }
  timer = millis();
  if (millis() == (timer + 250)) {
    digitalWrite(7, HIGH);
  }
  timer = millis();
  if (millis() == (timer + 250)) {
    digitalWrite(7, LOW);
  } 
  digitalWrite(7, HIGH);
  if (millis() == (timer + 250)) {
    digitalWrite(7, LOW);
  }
  timer = millis();
  if (millis() == (timer + 250)) {
    digitalWrite(7, HIGH);
  }
  timer = millis();
  if (millis() == (timer + 250)) {
    digitalWrite(7, LOW);
  } 
}

void logAlert (String myName, int zone, int alert) {

  // just blink the led and write a message out to the serial monitor

  //digitalWrite(13, HIGH);

  Serial.print("*** alert received at ");
  float sec = millis() / 1000.0;
  Serial.println(sec);
  Serial.print("***   node = ^");
  Serial.print(myName);
  Serial.print("^ in zone # ");
  Serial.print(zone);
  Serial.print(": ");
  Serial.println(myNode.zoneName(zone));
  Serial.print("***   alert = ");
  Serial.print(alert);
  Serial.print(": ");
  Serial.println(myNode.alertName(alert));

  // end of the blink
  //digitalWrite(13, LOW);

}

