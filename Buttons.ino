#define manualOverride 2
#define buttonScroll 3
#define buttonSelect A5 //A5 is shared with magnet sensor

void setupButtons() {
  pinMode(buttonScroll, INPUT);
  pinMode(buttonSelect, INPUT);
  pinMode(manualOverride, INPUT);
  attachInterrupts();
}

void attachInterrupts() {
  attachInterrupt(digitalPinToInterrupt(manualOverride), doManualOverride, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, RISING);
}

void detachInterrupts() {
  detachInterrupt(digitalPinToInterrupt(manualOverride));
  detachInterrupt(digitalPinToInterrupt(buttonScroll));
}

bool checkButton(uint8_t buttonState, bool * pButtonStateChanged) {
  if (buttonState == HIGH && *pButtonStateChanged) {
    *pButtonStateChanged = false;
    return true;
  }
  if (buttonState == LOW) {
    *pButtonStateChanged = true;
  }
  return false;
}

