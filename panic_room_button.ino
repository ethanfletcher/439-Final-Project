#include<Servo.h>
int pos = 90;

Servo servo;
int directionState = 0;
int buttonState = 0;

const int trapDoorBtn = 8;

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT);
  servo.attach(9);
}
void loop() {
  servo.write(pos);
  buttonState = digitalRead(trapDoorBtn);
  
  if (buttonState == HIGH) {
      for (pos=0; pos < 90; pos=pos+1); {
        Serial.println(pos);
        servo.write(pos); 
        delay(15); 
       // Serial.println(directionState);                          
      }
      for (pos=90; pos>=1; pos=pos-1); {
        Serial.println(pos);
        servo.write(pos); 
        delay(15); 
       // Serial.println(directionState);           
     }
  }
}
