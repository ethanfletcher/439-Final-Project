#include<Servo.h>
int pos = 90;

Servo servo;
int directionState = 0;
int buttonState = LOW;

const int trapDoorBtn = 8;

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT);
  servo.attach(9);
}
void loop() {
  servo.write(pos);
  buttonState = digitalRead(trapDoorBtn);
  
  if (buttonState == HIGH & pos==0) {
      for (pos=0; pos <= 89; pos=pos+1); {
        Serial.println(pos);
        servo.write(pos); 
        Serial.print("wrote");
        Serial.println(pos);
        delay(15);  
        buttonState =LOW;                       
      }
  }
  if (buttonState == HIGH & pos==90) {
      for (pos=90; pos>=1; pos=pos-1); {
        Serial.print("2loop");
        Serial.println(pos);
        servo.write(pos); 
        Serial.print("here");
        Serial.println(pos);
        delay(15); 
        buttonState = LOW;         
     }
  }
}
