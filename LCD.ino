#include <LiquidCrystal.h>

const int rs = 8, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long lastTempReading = 0;
int tempReadDelay = 1000;

void setupLCD() {
  lcd.begin(16, 2);
}

void printLcd() {
  switch (state) {
    case triggered:
      lcd.setCursor(0, 0);
      lcd.print(F("triggered!"));
      break;
    case menu:
      showMenu();
      break;
    default:
      clockWatch(tempReadDelay, &lastTempReading, []() {
        lcd.setCursor(0, 0);
        lcd.print(F("It's "));
        lcd.print(getTemperature());
        lcd.print(F(" degrees"));
        lcd.setCursor(0, 1);
        lcd.print(F("celcius."));
        // If the interrupt to go to menu fired, this shouldn't have been printed. clear it.
        if (state == menu) {
          clearLCD();
        }
      });
      break;
  }
}

void clearLCD() {
  lcd.clear();
}

