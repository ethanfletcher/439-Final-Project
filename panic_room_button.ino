#include<Servo.h>
int pos = 0;

Servo servo;
int directionState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  servo.attach(9);
}
void loop() {
  servo.write(pos);
  if (directionState == 0){
    if (digitalRead(2) == HIGH) {
      directionState = 1;
      for (pos=0; pos < 90; pos=pos+1); {
        Serial.println(pos);
        servo.write(pos); 
        delay(15); 
        Serial.println(directionState);                             
    }
    }
  }
    else if (directionState == 1) {
      if (digitalRead(2) == HIGH) {
        directionState = 0;
        for (pos=90; pos>=1; pos=pos-1); {
          Serial.println(pos);
          servo.write(pos); 
          delay(15); 
          Serial.println(directionState);           
     }
  }
  }
  }
