const uint8_t manualOverride = 2,
              buttonScroll = 3,
              buttonSelect = 15; //A1 as digital

enum MenuItems {
  sprayDelayMenu,
  spraysRemainingMenu,
  exitMenu
};

bool isInSubMenu;

uint8_t menuState;
bool scrollStateChanged;
bool selectStateChanged;
bool * pScrollStateChanged = &scrollStateChanged;
bool * pSelectStateChanged = &selectStateChanged;

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

void enterMenu() {
  detachInterrupt(digitalPinToInterrupt(buttonScroll));
  state = menu;
  menuState = sprayDelayMenu;
  clearLCD();
}

void checkButtons() {
  if (checkButton(buttonScroll, pScrollStateChanged)) {
    clearLCD();
    if (!isInSubMenu) {    // If we are not in a submenu, we scroll through the menu.
      menuState++;
      if (menuState > 2) { // it wraps around
        menuState = 0;
      }
    }
    else {     // if we are in a submenu, do submenu things:
      switch (menuState) {
        case sprayDelayMenu: // raise the spray delay...
          sprayDelay *= 2;
          if (sprayDelay > 32000) { // max 32 second delay
            sprayDelay = 0;
          }
          break;
        case spraysRemainingMenu: // Or cancel a reset
          isInSubMenu = !isInSubMenu;
          break;
      }
    }
  }

  if (checkButton(buttonSelect, pSelectStateChanged)) {
    clearLCD();
    switch (menuState) {
      case exitMenu: // exit selected, get back out of menu.
        attachInterrupt(digitalPinToInterrupt(buttonScroll), enterMenu, FALLING);
        state = notInUse;
        break;
      case sprayDelayMenu:  //right spray delay selected by user, or selected to go into submenu.
        isInSubMenu = !isInSubMenu;
        break;
      case spraysRemainingMenu:
        if (isInSubMenu) { // user selected to do a reset
          resetSpraysRemaining();
        }
        isInSubMenu = !isInSubMenu; //... or selected the sprays remaining option.
        break;
    }
  }
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

void resetSpraysRemaining() {
  spraysRemaining = 2400;
}

