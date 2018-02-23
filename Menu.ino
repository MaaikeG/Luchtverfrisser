enum MenuItems {
  sprayDelayMenu,
  spraysRemainingMenu,
  exitMenu
};

bool isInSubMenu;

uint8_t menuState;
bool scrollStateChanged;
bool selectStateChanged;

void enterMenu() {
  detachInterrupt(digitalPinToInterrupt(buttonScroll));
  state = menu;
  menuState = sprayDelayMenu;
  clearLCD();
}

void showMenu(){
  lcd.setCursor(0, 0);
  switch (menuState) {
    case sprayDelayMenu:
      if (isInSubMenu) {
        lcd.print(sprayDelay / 1000);
        lcd.setCursor(0, 1);
        lcd.print(F("save       >"));
      }
      else {
        lcd.print(F("Spray delay:"));
        lcd.setCursor(0, 1);
        lcd.print(sprayDelay / 1000);
      }
      break;
    case spraysRemainingMenu:
      if (isInSubMenu) {
        lcd.print(F("Reset sprays left?"));
        lcd.setCursor(0, 1);
        lcd.print("<no     yes>");
      }
      else {
        lcd.print(F("Sprays left:"));
        lcd.setCursor(0, 1);
        lcd.print(spraysRemaining);
      }
      break;
    case exitMenu:
      lcd.print(F("Exit"));
      break;
  }
}

void checkButtons() {
  if (checkButton(buttonScroll, &scrollStateChanged)) {
    clearLCD();
    if (!isInSubMenu) {    // If we are not in a submenu, we scroll through the menu.
      menuState++;
      if (menuState > exitMenu) { // it wraps around
        menuState = sprayDelayMenu;
      }
    }
    else {     // if we are in a submenu, do submenu things:
      switch (menuState) {
        case sprayDelayMenu: // raise the spray delay...
          sprayDelay += 1000;
          if (sprayDelay > maxSprayDelay) {
            sprayDelay = 0;
          }
          break;
        case spraysRemainingMenu: // Or cancel a reset
          isInSubMenu = !isInSubMenu;
          break;
      }
    }
  }

  if (checkButton(buttonSelect, &selectStateChanged)) {
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
  spraysRemaining = startSpraysRemaining;
}
