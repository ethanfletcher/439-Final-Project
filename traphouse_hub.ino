/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <LiquidCrystal.h>

Servo myServo1;  // create servo object to control drawbridge servo
Servo myServo2; // servo object that controls panic room door

int posServo1 = 0;    // variable to store the servo position
int posServo2 = 90; // variable for servo controlling panic room door

const int hallPin = 12; // Hall effect sensor
const int moatButtonPin = 4; // Button to control moat drawbridge
const int switchPin = 2; // Door magnetic sensor
const int ledPin = 13;
const int trapDoorBtn = 8; // panic room button

const int analogInPin = A0; // analog input pi tat potentiometer is attached to
const int analogOutPin = 9; // analog out pin that the LED is attached to

int lcdPotValue = 0;
int lcdOutputValue = 0;
int hallState = 0;
int moatButtonState = 0;
int panicButtonState = LOW; // panic room button state

void setup() {
  myServo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myServo2.attach(10); // attaches panic room servo on pin 10
  Serial.begin(9600);
  pinMode(hallPin, INPUT);
  pinMode(moatButtonPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trapDoorBtn, INPUT);
  digitalWrite(switchPin, HIGH);
}

void loop() {
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
}

