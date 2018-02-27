#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 12, 4, 5, 11, 7);

unsigned long lastDisplayChange;
#define tempRefreshSeconds 2

void setupLCD() {
  lcd.begin(16, 2);
}

void setLCD() {
  lcd.clear();
  switch (state) {
    case triggered:
      lcd.setCursor(0, 0);
      lcd.print(F("Spraying in "));
      printCountDown();
      lcd.setCursor(0,1);
      lcd.print(nSprays); 
      lcd.print(" spray(s)!");
      break;
    case menu:
      showMenu();
      break;
    default:
      lcd.setCursor(0, 0);
      lcd.print(F("It's "));
      lcd.print(getTemperature());
      lcd.print(F(" degrees"));
      lcd.setCursor(0, 1);
      lcd.print(spraysRemaining);
      lcd.print(" sprays left");
      break;
  }
}

void printTemperature() {
  clockWatch(tempRefreshSeconds * 1000, &lastDisplayChange, []() {
    lcd.setCursor(5, 0);
    lcd.print(getTemperature());
  });
}

void printTriggerCountDown() {
  clockWatch(1000, &lastDisplayChange, []() {
    printCountDown();
  });
}

void printCountDown() {
  lcd.setCursor(12,0);
  int timeLeft = (sprayDelay - (millis() - triggeredAt))/1000;
  timeLeft = timeLeft < 0? 0 : timeLeft;
  lcd.print(timeLeft);
}
    
