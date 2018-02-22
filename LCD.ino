#include <LiquidCrystal.h>

const int rs = 8, en = 13, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setupLCD() {
  lcd.begin(16, 2);
}

void printLcd() {  
  switch (state) {
    case triggered:
      lcd.setCursor(0,0);
      lcd.print(F("triggered!"));
    break;
    case menu: 
      lcd.setCursor(0,0);
      switch (menuState){
        case sprayDelayMenu:
          if(isInSubMenu) {
            lcd.print(sprayDelay / 1000);
            lcd.setCursor(0,1);
            lcd.print(F("save       >"));
          }
          else {
            lcd.print(F("Spray delay:"));
            lcd.setCursor(0,1);
            lcd.print(sprayDelay / 1000);
          }
        break;
        case spraysRemainingMenu:
           if(isInSubMenu) {
            lcd.print(F("Reset sprays left?"));
            lcd.setCursor(0,1);
            lcd.print("<no     yes>");
            }
           else {
            lcd.print(F("Sprays left:"));
            lcd.setCursor(0,1);
            lcd.print(spraysRemaining);
           }
        break;
        case exitMenu:
          lcd.print(F("Exit"));
        break;
      }
      break;
      default:
        lcd.setCursor(0,0);
        lcd.print(F("It's ")); 
        lcd.print(getTemperature()); 
        lcd.print(F(" degrees"));
        lcd.setCursor(0,1);
        lcd.print(F("celcius."));
        // If the interrupt to go to menu fired, this shouldn't have been printed. clear it.
        if(state == menu) {
          clearLCD();
        }
      break;
   }
}

void clearLCD() {
  lcd.clear();
}

