#define red 9
#define green 10
#define blue 6
uint8_t rgbBrightness[3];
uint8_t fadeAmount = 1;
uint8_t  maxBrightness = 50;

unsigned long previousUpdateMillis;

void setupLEDs() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void setLEDColor() {
  fadeAmount = 5;
  rgbBrightness[0] = 0;
  rgbBrightness[1] = 0;
  rgbBrightness[2] = 0;

  switch (state) {
    case useUnknown:
      rgbBrightness[1] = 150; //green light on
      break;
    case type1Use:
      rgbBrightness[0] = 255; // orange light on
      rgbBrightness[1] = 40; // green is way brighter than red, so lower value required
      break;
    case type2Use:
      rgbBrightness[0] = 255; // red light on
      break;
    case cleaning:
      rgbBrightness[2] = 255; // blue light on
      break;
    case triggered:
      rgbBrightness[0] = 255; // setup for disco, start with red.
      rgbBrightness[1] = 0;
      rgbBrightness[2] = 0;
      break;
  }
  outputLEDs();
}

byte decColour = 0;
byte incColour = 1;

void changeLEDcolor() {
  switch (state) {
    case notInUse:
      clockWatch(1000, &previousUpdateMillis, []() {
        rgbBrightness[1] = rgbBrightness[1] == 0 ? maxBrightness : 0;
        outputLEDs();
      });
      break;
    case menu:
      clockWatch(500, &previousUpdateMillis, []() {
        if (rgbBrightness[0] == 0) {
          rgbBrightness[0] = 255;
        }
        else {
          rgbBrightness[0] = 0;
        }
        outputLEDs();
      });
      break;
    case triggered: // Do the disco!
      if (rgbBrightness[incColour] >= 255) {
        decColour++;
        decColour = decColour % 3;
      }
      incColour = decColour == 2 ? 0 : decColour + 1;

      clockWatch(5, &previousUpdateMillis, []() {
        rgbBrightness[decColour] -= fadeAmount;
        rgbBrightness[incColour] += fadeAmount;
        outputLEDs();
      });
      break;
  }
}

void outputLEDs() {
  analogWrite(red, rgbBrightness[0]);
  analogWrite(green, rgbBrightness[1]);
  analogWrite(blue, rgbBrightness[2]);
}

