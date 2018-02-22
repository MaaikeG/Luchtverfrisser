const uint8_t red = 9, green = 10, blue = 11;
const uint8_t maxBrightness = 5;
int brightness = 0;
int fadeAmount = 1;

void setupLEDs() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void setLedColor() {
  switch (state) {
    case notInUse:
      clockWatch(300, &previousUpdateMillis, []() {
        brightness = brightness + fadeAmount;
        analogWrite(green, brightness);

        if (brightness <= 0 || brightness >= maxBrightness)
          fadeAmount = -fadeAmount;
      });

      // todo: state transition logic and resetting all the
      // hardware we used (e.g. turn the led off)
      break;
    case useUnknown:
      if (stateChanged)
        analogWrite(green, maxBrightness);
      break;
  }
}
