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
      lcd.print("triggered!");
    break;
    case menu: 
      lcd.setCursor(0,0);
      switch (menuState){
        case sprayDelayMenu:
          if(isInSubMenu) {
            lcd.print(sprayDelay / 1000);
            lcd.setCursor(0,1);
            lcd.print("save       >");
          }
          else {
            lcd.print("Spray delay:");
            lcd.setCursor(0,1);
            lcd.print(sprayDelay / 1000);
          }
        break;
        case spraysRemainingMenu:
           if(isInSubMenu) {
            lcd.print("Reset sprays left?");
            lcd.setCursor(0,1);
            lcd.print("<no     yes>");
            }
           else {
            lcd.print("Sprays left:");
            lcd.setCursor(0,1);
            lcd.print(spraysRemaining);
           }
        break;
        case exitMenu:
          lcd.print("Exit");
        break;
      }
      break;
      default:
        lcd.setCursor(0, 1);
        lcd.print("Temp: " + getTemperatureString());
      break;
   }
}

void clearLCD() {
  lcd.clear();
}

