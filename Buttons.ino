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
  detachInterrupt(buttonScroll);
  state = menu;
  menuState = sprayDelay;
}

void checkButtons() {
  int scrollButtonState = debouncedDigitalRead(buttonScroll);
  if (scrollButtonState == LOW && !menuStateChanged) {
    menuStateChanged = true;
    menuState++;
    if (state >= 2)
      state = 0;
  } else if (scrollButtonState == HIGH)
    menuStateChanged = false;
}


