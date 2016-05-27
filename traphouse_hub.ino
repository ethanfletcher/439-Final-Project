/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <SmartCityNodeLib.h>

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
//const int switchPin = 2; // Door magnetic sensor
const int ledPin = 7;
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

void setup() {
  myServo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myServo2.attach(10); // attaches panic room servo on pin 10
  Serial.begin(9600);
  pinMode(hallPin, INPUT);
  pinMode(moatButtonPin, INPUT);
//  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trapDoorBtn, INPUT);
  pinMode(magneticSwitchPin, INPUT);
  digitalWrite(magneticSwitchPin, HIGH);
//  digitalWrite(switchPin, HIGH);

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
  int alert = myNode.alertReceived();
  if (alert != 0) {
    // in this demo, we'll just print out a message about it
    logAlert(myNodeName, myZone, alert);
    // here is where you would do the real work of processing this alert:
    if (alert == SmartCityNode::BLACK_FRIDAY || alert == SmartCityNode::FIRE) {
      digitalWrite(ledPin, HIGH);
      delay(2000);
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
  
  hallState = digitalRead(hallPin);
  moatButtonState = digitalRead(moatButtonPin);
  panicButtonState = digitalRead(trapDoorBtn);

  myServo2.write(posServo2);

  if (hallState == LOW || moatButtonState == HIGH) {
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

  if (panicButtonState == HIGH & posServo2==0) {
      for (posServo2=0; posServo2 <= 89; posServo2=posServo2+1); {
        Serial.println(posServo2);
        myServo2.write(posServo2); 
        Serial.print("wrote");
        Serial.println(posServo2);
        delay(15);  
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

  if(digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 0) {
    digitalWrite(ledPin, HIGH);
    delay(5000);
    magneticDirectionState = 1;
  }
  if (digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 1){
    digitalWrite(ledPin, LOW);
    delay(5000);
    magneticDirectionState = 0;
  }
}
void logAlert (String myName, int zone, int alert) {

  // just blink the led and write a message out to the serial monitor

  digitalWrite(13, HIGH);

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
  digitalWrite(13, LOW);

}

