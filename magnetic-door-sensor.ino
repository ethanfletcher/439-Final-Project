  runTime = millis();
  if(digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 0) {
    digitalWrite(ledPin, HIGH);
    if (millis() >= (runTime + 5000)) {
      magneticDirectionState = 1;
    }
  }
  runTime = millis();
  if (digitalRead(magneticSwitchPin) == HIGH & magneticDirectionState == 1){
    digitalWrite(ledPin, LOW);
    if (millis() >= (runTime + 5000)) {
      magneticDirectionState = 0;
    }
  }
