/* #include <SmartCityNodeLib.h>

http://www.instructables.com/id/Magnetic-Door-Sensor-and-Arduino/?ALLSTEPS

*/

const int magneticSwitchPin = 5;
const int ledPin = 13;
int magneticDirectionState = 0;
int ledState = LOW;

void setup() {
pinMode(magneticSwitchPin, INPUT);
pinMode(ledPin, OUTPUT);
digitalWrite(magneticSwitchPin, HIGH);
digitalWrite(ledPin, LOW);
}

void loop() {
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
}
