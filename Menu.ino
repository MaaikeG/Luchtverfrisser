const uint16_t maxSprayDelay = 32000;
const uint16_t startSpraysRemaining = 2400;

bool scrollStateChanged;
bool selectStateChanged;
bool * pScrollStateChanged = &scrollStateChanged;
bool * pSelectStateChanged = &selectStateChanged;

uint8_t menuState;
bool isInSubMenu;
enum MenuItems {
  sprayDelayMenu,
  spraysRemainingMenu,
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
    case exitMenu:
      lcd.print(F("Exit"));
      break;
  }
}

void doManualOverride() {
  if(state != triggered){
    state = triggered;
  }
}

void enterMenu() {
  if (state != menu) {
    //go to the exit menu, because a scroll press will be detected and we wrap around
    menuState = exitMenu;
    state = menu;
  }
}

void checkButtons() {
  if (checkButton(buttonScroll, pScrollStateChanged)) {
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
          if (sprayDelay > maxSprayDelay) { // max 32 second delay
            sprayDelay = 0;
          }
          break;
        case spraysRemainingMenu: // Or cancel a reset
          isInSubMenu = !isInSubMenu;
          break;
      }
    }
    showMenu();
  }

  if (checkButton(buttonSelect, pSelectStateChanged)) {
    switch (menuState) {
      case exitMenu: // exit selected, get back out of menu.
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
    showMenu();
  }
}

void resetSpraysRemaining() {
  spraysRemaining = startSpraysRemaining;
}
