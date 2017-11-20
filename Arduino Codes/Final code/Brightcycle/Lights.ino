
//Blinkers function: the pin is the number of the pin of the blinker we want to turn on 
void blinking(int ledPin) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

void TurnOff(int Pin){
  digitalWrite(Pin, LOW);
}

void TurnOn(int Pin){
  digitalWrite(Pin, HIGH);
}

void manualLight(int value){
  value = value*2;
  analogWrite(BackLightPin, value);
  //Serial.print("Value:");Serial.println(value);
}
