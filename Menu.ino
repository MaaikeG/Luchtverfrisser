#define maxSprayDelay 32000
#define maxNumberOfSprays 5

bool scrollStateChanged;
bool selectStateChanged;
bool * pScrollStateChanged = &scrollStateChanged;
bool * pSelectStateChanged = &selectStateChanged;

uint8_t menuState;
bool isInSubMenu;

enum MenuItems {
  sprayDelayMenu,
  spraysRemainingMenu,
  sprayAmountType1Use,
  sprayAmountType2Use,
  sprayAmountOverride,
  exitMenu
};

void showMenu() {
  lcd.clear();
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
    case sprayAmountType1Use:
      printSprayAmount(nSpraysUse1, "number 1: ");
      break;
    case sprayAmountType2Use:
      printSprayAmount(nSpraysUse2, "number 2: ");
      break;
    case sprayAmountOverride:
      printSprayAmount(nSpraysOverride, "manual spray: ");
      break;
    case exitMenu:
      lcd.print(F("Exit"));
      break;
  }
}

void doManualOverride() {
  if (state != triggered) {
    trigger(nSpraysOverride);
  }
}

void enterMenu() {
  if (state != menu) {
    //go to the exit menu, because a scroll press will be detected and we wrap around
    menuState = exitMenu;
    detachInterrupts();
    setNewState(menu);
  }
}

void printSprayAmount(uint8_t nSprays, String lcdText){
  if (isInSubMenu) {
    lcd.print(F("Change sprays"));
  }
  else {
    lcd.print(F("Sprays for a"));
  }
  lcd.setCursor(0, 1);
  lcd.print(lcdText);
  lcd.print(nSprays);
}

void doExitMenu() {
  attachInterrupts();
  setNewState(notInUse);
}

void checkButtons() {
  if (checkButton(debouncedDigitalRead(buttonScroll), pScrollStateChanged)) {
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
        case sprayAmountType1Use: // raise the spray delay...
          increaseNSprays(&nSpraysUse1);
          break;
        case sprayAmountType2Use:
          increaseNSprays(&nSpraysUse2);
          break;
        case sprayAmountOverride:
          increaseNSprays(&nSpraysOverride);
          break;
      }
    }
    stateChanged = true;
  }

  if (checkButton(debouncedAnalogRead(buttonSelect, 0), pSelectStateChanged)) {
    switch (menuState) {
      case exitMenu: // exit selected, get back out of menu.
        doExitMenu();
        break;
      case spraysRemainingMenu:
        if (isInSubMenu) { // user selected to do a reset
          resetSpraysRemaining();
        }
        isInSubMenu = !isInSubMenu; //... or selected the sprays remaining option.
        break;
      default:
        isInSubMenu = !isInSubMenu;
        break;
    }
    stateChanged = true;
  }
}

void increaseNSprays(uint8_t * nSprays) {
  *nSprays += 1;
  if (*nSprays > maxNumberOfSprays) {
    *nSprays = 0;
  }
}

