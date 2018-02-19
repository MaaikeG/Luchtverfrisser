const uint8_t freshenerPin = 12; //A1 as digital

const uint8_t debounceDelay = 50;
// this might go wrong if multiple buttons are pressed at the same time
// , but having only one of these saves memory.
unsigned long lastDebounceTime;
int lastButtonState;

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
unsigned long previousUpdateMillis;

void setup() {
  Serial.begin(9600);
  setupLEDs();
  setupLCD();
  setupSensors();
  setupButtons();
  pinMode(freshenerPin, OUTPUT);
}

void loop() {
  printLcd();
  setLedColor();
  switch (state) {
    case menu:
      checkButtons();
  }
//  int buttonState = debouncedDigitalRead(overridePin);
//  if (buttonState == LOW && !stateChanged) {
//    stateChanged = true;
//    state++;
//    // reset if we exceed the number of implemented states
//    if (state >= 2)
//      state = 0;
//  } else if (buttonState == HIGH)
//    stateChanged = false;
}

void clockWatch(int frequency, unsigned long* lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    previousUpdateMillis = millis();
    (*f)();
  }
}

bool debouncedDigitalRead(int buttonPin) {
  int buttonState = HIGH;
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  return buttonState;
}
