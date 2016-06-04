#include <Servo.h>
#include <SoftwareSerial.h>
#include <SmartCityNodeLib.h>
#include "Adafruit_LiquidCrystal.h"
#include "Wire.h"

Adafruit_LiquidCrystal lcd(0);

// some constants to identify my node to the library object
const String myNodeName = "1017 Trap House";
const int myZone = SmartCityNode::RESIDENTIAL;  

SmartCityNode myNode;
boolean myDebugging = true;

Servo myServo1;  // create servo object to control drawbridge servo
Servo myServo2; // servo object that controls panic room door

int posServo1 = 90;    // variable to store the servo position
int posServo2 = 90; // variable for servo controlling panic room door

const int hallPin = 12; // Hall effect sensor
const int moatButtonPin = 4; // Button to control moat drawbridge
const int ledPin = 13;
const int trapDoorBtn = 8; // panic room button
const int magneticSwitchPin = 5; // magnetic contact switch door sensor pin

int hallState = 0;
int moatButtonState = 0;
int panicButtonState = LOW; // panic room button state
int magneticDirectionState = 0;
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  myServo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myServo2.attach(10); // attaches panic room servo on pin 10
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

  myServo2.write(posServo2);
  myServo1.write(posServo1);

  Serial.println("\n\n*** Starting SmartCityNode demo");

  // just flash it for a second at startup so we know things have initialized
  digitalWrite(13, HIGH);
  delay(1000); 
  digitalWrite(13, LOW);

  myNode.setDebugOn();

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
    logAlert(myNodeName, myZone, alert);
    // responding to alerts
    if (alert == SmartCityNode::HIPSTER_INVASION) {
      lcd.setCursor(0, 0);
      lcd.print("Hipsters Detected!");
      flashLED();
    }
    if (alert == SmartCityNode::BURGLARY) {
      lcd.setCursor(0, 0);
      lcd.print("Burglary Detected!");
      openPanicRoom();
    }
    if (alert == SmartCityNode::FIRE) {
      lcd.setCursor(0, 0);
      lcd.print("Fire Detected!");
      openDrawBridge();
    }
  }
  // End alert responses

  myServo2.write(posServo2);
  myServo1.write(posServo1);
  
// Servo Drawbridge controlled by button and hall effect sensor
  if (hallState == 1 || moatButtonState == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Drawbridge");
    Serial.println(hallState);
    Serial.println("Magnet detected");
    Serial.println("Rotation started");
    for (posServo1 = 90; posServo1 >= 0; posServo1 -= 1) { // goes from closed to open
      myServo1.write(posServo1);             
      delay(15);                     
      if (posServo1 == 89) {
        delay(3000);
      }
    }
  }
  delay(5);
// End drawbridge code

// servo panic room door controlled by button
  if (panicButtonState == HIGH & posServo2==0) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Panic Room");
    for (posServo2=0; posServo2 <= 89; posServo2=posServo2+1) {
      Serial.println(posServo2);
      myServo2.write(posServo2); 
      delay(50);  
      panicButtonState =LOW;                       
    }
  }
  if (panicButtonState == HIGH & posServo2==90) {
    for (posServo2=90; posServo2>=1; posServo2=posServo2-1) {
      Serial.println(posServo2);
      myServo2.write(posServo2); 
      delay(15); 
      panicButtonState = LOW;         
    }
  }
// end panic room button code

// Magnetic door sensor 
  if(digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 0) {
    ledState = HIGH;
    digitalWrite(ledPin, HIGH);
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
  lcd.setCursor(0, 1);
  lcd.print("Drawbridge");
  Serial.println("Magnet detected");
  Serial.println("Rotation started");
  myServo1.write(0);
  delay(5000);
  for (posServo1 = 0; posServo1 <= 90; posServo1 += 1) { // goes from closed to open
    myServo1.write(posServo1);            
    delay(15);                      
    if (posServo1 == 89) {
      delay(3000);
    }
  }
  lcd.clear();
  delay(2000);
}

void openPanicRoom() { // Opens then closes panic room door
  lcd.setCursor(0, 1);
  lcd.print("Panic Room");
  for (posServo2=0; posServo2 <= 89; posServo2=posServo2+1) {
    Serial.println(posServo2);
    myServo2.write(posServo2); 
    delay(15);  
    panicButtonState =LOW;                       
  }
  for (posServo2=90; posServo2>=1; posServo2=posServo2-1) {
    Serial.println(posServo2);
    myServo2.write(posServo2); 
    delay(15); 
    panicButtonState = LOW;         
  }  
}

void flashLED() {
  Serial.println("FLASHING LEDS!");
  lcd.setCursor(0, 0);
  lcd.print("Flashing LEDs");
  digitalWrite(7, HIGH);
  delay(250);
  digitalWrite(7, LOW);
  delay(250);
  digitalWrite(7, HIGH);
  delay(250);
  digitalWrite(7, LOW);
  delay(250);  
}

void logAlert (String myName, int zone, int alert) {
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
}

