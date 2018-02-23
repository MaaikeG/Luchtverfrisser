const uint8_t manualOverride = 2,
              buttonScroll = 3,
              buttonSelect = 15; //A1 as digital

void setupButtons() {
  pinMode(buttonScroll, INPUT);
  pinMode(buttonSelect, INPUT);
  pinMode(manualOverride, INPUT);
  attachInterrupt(digitalPinToInterrupt(manualOverride), doManualOverride, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, FALLING);
}

void doManualOverride() {
  if (state = menu) {
    attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, FALLING);
  }
  clearLCD();
  state = triggered;
}



