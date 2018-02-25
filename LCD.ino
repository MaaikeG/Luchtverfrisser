#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 12, 4, 5, 6, 7);

unsigned long lastTempReading;
#define tempRefreshSeconds 2

void setupLCD() {
  lcd.begin(16, 2);
}

void setLCD() {
  lcd.clear();
  switch (state) {
    case triggered:
      lcd.setCursor(0, 0);
      lcd.print(F("triggered!"));
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
  clockWatch(tempRefreshSeconds * 1000, &lastTempReading, []() {
    lcd.setCursor(5, 0);
    lcd.print(getTemperature());
  });
}

