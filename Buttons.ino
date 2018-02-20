const uint8_t manualOverride = 2, 
              buttonScroll = 3,
              buttonSelect = 15; //A1 as digital

enum MenuItems {
  sprayDelay,
  spraysRemaining,
  exitMenu
};
uint8_t menuState;
bool menuStateChanged;

void setupButtons() {
  pinMode(buttonScroll, INPUT);
  pinMode(buttonSelect, INPUT); 
  pinMode(manualOverride, INPUT);
  attachInterrupt(digitalPinToInterrupt(manualOverride), doManualOverride, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, FALLING);
}

void doManualOverride() {
  state = triggered;
}

void enterMenu() {
  detachInterrupt(digitalPinToInterrupt(buttonScroll));
  state = menu;
  menuState = sprayDelay;
  menuStateChanged = true;
}

void checkButtons() {
  int scrollButtonState = debouncedDigitalRead(buttonScroll);
  if (scrollButtonState == LOW && !menuStateChanged) {
    menuStateChanged = true;
    menuState++;
    Serial.println(menuState);
    if (menuState > 2) {
      menuState = 0;
    }
  } 
  if (scrollButtonState == HIGH)
    menuStateChanged = false;
}


