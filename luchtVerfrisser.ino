const uint8_t freshenerPin = 13;

const uint8_t debounceDelay = 50;
unsigned long lastDebounceTime;

int lastButtonStates;
int currentButtonStates; // contains current and last states of each button.
// The bit on the position that is also the index of the button contains
// the last state for that button.

// TODO: Save these is EEPROM!!!
int sprayDelay = 15000; // delay in ms
uint16_t spraysRemaining = 2400;

enum State {
  notInUse,
  useUnknown,
  type1Use,
  type2Use,
  cleaning,
  triggered,
  menu
};

enum State state = notInUse;
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
}

void clockWatch(int frequency, unsigned long* lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    *lastRunMillis = millis();
    (*f)();
  }
}

bool debouncedDigitalRead(int buttonPin) {
  uint8_t bitPosition = buttonPin / 2;
  int currentState = (currentButtonStates & (1 << bitPosition)) > 0;
  int lastState = (lastButtonStates & (1 << bitPosition)) > 0;

  int reading = digitalRead(buttonPin);

  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    if (reading != currentState) {
      currentState = reading;
      currentButtonStates ^= (-reading ^ currentButtonStates) & (1UL << bitPosition);
    }
  }
  // save reading for next time in the loop.
  lastButtonStates ^= (-reading ^ lastButtonStates) & (1UL << bitPosition);

  return currentState;
}



