const uint8_t redPin = 9, greenPin = 10, bluePin = 11,
              freshenerPin = 12,
              overridePin = 14; //A1 as digital

enum State {
  notInUse,
  useUnknown,
  type1Use,
  type2Use,
  cleaning,
  triggered,
  menu
};

uint8_t state = notInUse;
bool stateChanged;

// notInUse variables
int brightness = 0;
int fadeAmount = 5;
unsigned long previousUpdateMillis;

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(freshenerPin, OUTPUT);
  pinMode(overridePin, INPUT);
}

void loop() {
  int buttonState = digitalRead(overridePin);
  if(buttonState == LOW && !stateChanged){
    stateChanged = true;
    state++;
    // reset if we exceed the number of implemented states
    if(state >= 2)
      state = 0;
  }else if(buttonState == HIGH)
    stateChanged = false;
  
  switch(state){
    case notInUse:
      clockWatch(30, &previousUpdateMillis, [](){
        brightness = brightness + fadeAmount;
        analogWrite(greenPin, brightness);
        
        if (brightness <= 0 || brightness >= 255)
          fadeAmount = -fadeAmount;
      });

      // todo: state transition logic and resetting all the 
      // hardware we used (e.g. turn the led off)
      break;
    case useUnknown:
      if(stateChanged)
        digitalWrite(greenPin, HIGH);
      break;
  }
}

void clockWatch(int frequency, unsigned long* lastRunMillis, void (*f)()){
  if(millis() - *lastRunMillis >= frequency){
    previousUpdateMillis = millis();
    (*f)();
  }
}

