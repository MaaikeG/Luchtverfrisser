#define manualOverride 2
#define buttonScroll 3
#define buttonSelect 15 //A1 as digital

void setupButtons() {
  pinMode(buttonScroll, INPUT);
  pinMode(buttonSelect, INPUT);
  pinMode(manualOverride, INPUT);
  attachInterrupt(digitalPinToInterrupt(manualOverride), doManualOverride, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, FALLING);
}

bool checkButton(uint8_t button, bool * pButtonStateChanged) {
  uint8_t buttonState = debouncedDigitalRead(button);

  if (buttonState == LOW && *pButtonStateChanged) {
    *pButtonStateChanged = false;
    return true;
  }
  if (buttonState == HIGH) {
    *pButtonStateChanged = true;
  }
  return false;
}

