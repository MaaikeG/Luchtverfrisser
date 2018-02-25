#include <LiquidCrystal.h>

const int rs = 8, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long lastTempReading;
unsigned int tempReadDelaySeconds = 30;

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
      lcd.print(F("  degrees"));
      lcd.setCursor(0, 1);
      lcd.print(F("celcius."));
      break;
  }
}

void printTemperature() {
  clockWatch(tempReadDelaySeconds * 1000, &lastTempReading, []() {
    lcd.setCursor(5, 0);
    lcd.print(getTemperature());
  });
}

