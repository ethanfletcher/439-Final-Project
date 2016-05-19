/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myServo1;  // create servo object to control a servo

int posServo1 = 0;    // variable to store the servo position

const int hallPin = 12; // Hall effect sensor
const int moatButtonPin = 7; // Button to control moat drawbridge
const int switchPin = 2; // Door magnetic sensor
const int ledPin = 13;
int hallState = 0;
int moatButtonState = 0;

void setup() {
  myServo1.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  pinMode(hallPin, INPUT);
  pinMode(moatButtonPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(switchPin, HIGH);
}

void loop() {
  hallState = digitalRead(hallPin);
  moatButtonState = digitalRead(moatButtonPin);

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
}

