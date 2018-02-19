const uint8_t red = 9, green = 10, blue = 11;
int brightness = 0;
int fadeAmount = 5;

void setupLEDs() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void setLedColor() {
    switch(state){
    case notInUse:
      clockWatch(30, &previousUpdateMillis, [](){
        brightness = brightness + fadeAmount;
        analogWrite(green, brightness);
        
        if (brightness <= 0 || brightness >= 255)
          fadeAmount = -fadeAmount;
      });

      // todo: state transition logic and resetting all the 
      // hardware we used (e.g. turn the led off)
      break;
    case useUnknown:
      if(stateChanged)
        digitalWrite(green, HIGH);
      break;
    }
}
