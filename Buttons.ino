const uint8_t manualOverride = 2, 
              buttonScroll = 3,
              buttonSelect = 15; //A1 as digital

unsigned long lastInterruptFired;

void setupButtons() {
  pinMode(buttonScroll, INPUT);
  pinMode(buttonSelect, INPUT); 
  pinMode(manualOverride, INPUT);
  attachInterrupts();
}

void attachInterrupts() {
  lastInterruptFired = millis();
  attachInterrupt(digitalPinToInterrupt(manualOverride), doManualOverride, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, FALLING);
}

void detachInterrupts() {
  detachInterrupt(digitalPinToInterrupt(manualOverride));
  detachInterrupt(digitalPinToInterrupt(buttonScroll));
} 

bool checkButton(int button, bool * pButtonStateChanged) {
  int buttonState = debouncedDigitalRead(button);

  if (buttonState == LOW && *pButtonStateChanged) {
    *pButtonStateChanged = false;
    return true;
  } 
  if (buttonState == HIGH) {
    *pButtonStateChanged = true;
  }
  return false;
}

