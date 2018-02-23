const uint8_t red = 9, green = 10, blue = 11;
int rgbBrightness[3];
int fadeAmount = 5;

void setupLEDs() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void setLEDColor() {
  rgbBrightness[0] = 0; rgbBrightness[1] = 0; rgbBrightness[2] = 0;
    switch(state){
    case useUnknown:
      rgbBrightness[1] = 255; //green light on
      break;
    case type1Use:
      rgbBrightness[0] = 255; // orange light on
      rgbBrightness[1] = 255; 
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
      clockWatch(20, &previousUpdateMillis, [](){
        rgbBrightness[1] = rgbBrightness[1] + fadeAmount;        
        if (rgbBrightness[1] <= 0 || rgbBrightness[1] >= 255)
          fadeAmount = -fadeAmount;
      });
      break;
    case triggered:
      incColour = decColour == 2 ? 0 : decColour + 1;
        
      clockWatch(5, &previousUpdateMillis, [](){
        rgbBrightness[decColour] -= fadeAmount;
        rgbBrightness[incColour] += fadeAmount;

        if (rgbBrightness[incColour] == 255 || rgbBrightness[decColour] == 0) {
          decColour = incColour;
          decColour ++;
        }
      });
    break;
  }
}

void fadeLED(int LED, int * brightness, int fadeAmount) {
    *brightness = *brightness + fadeAmount;
    analogWrite(LED, *brightness);
    if (*brightness <= 0 || *brightness >= 200)
          fadeAmount = -fadeAmount;
}

void outputLEDs() {
  analogWrite(red, rgbBrightness[0]);
  analogWrite(green, rgbBrightness[1]);
  analogWrite(blue, rgbBrightness[2]);
}

