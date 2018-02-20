#include <LiquidCrystal.h>

const int rs = 8, en = 13, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setupLCD() {
  lcd.begin(16, 2);
}

void printLcd() {  
  if(state == triggered) {
      lcd.setCursor(0,0);
      lcd.print("triggered!");
  }
  else if (state == menu) {
    lcd.setCursor(0,0);
    switch (menuState){
      case sprayDelay:
        lcd.print("Spray delay:");
      break;
      case spraysRemaining:
        lcd.print("Sprays left:");
      break;
      case exitMenu:
        lcd.print("Exit");
      break;
      }
      lcd.setCursor(0,1);
      lcd.print(">");
    }
   else {
      lcd.setCursor(0, 1);
      lcd.print("Temp: " + getTemperatureString());
   }
}


