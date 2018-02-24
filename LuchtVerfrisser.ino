const uint8_t freshenerPin = 13;

// TODO: Save these in EEPROM!!!
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
//uint8_t menuState;
bool stateChanged;

void setup() {
  Serial.begin(9600);
  setupLEDs();
  setupLCD();
  setupSensors();
  setupButtons();
  pinMode(freshenerPin, OUTPUT);
  stateChanged = true;
  getTemperature();
}

void loop() {
  if (stateChanged) { // handle a state change
    doStateTransition();
    stateChanged = false;
  }
  detectStuff();

  changeLEDcolor();
  
  switch (state) {
    case notInUse:
      printTemperature();
      if (getMotionDetectorState()) {
        setNewState(useUnknown);
      }
      break;
    case useUnknown:  
      if (millis - getLastMotionDetected() > 5000 && (getDistance() > 150 || getDistance() == 0)) {
        setNewState(notInUse);
      }
      break;
    case cleaning:
      if (millis - getLastMotionDetected() > 5000 && (getDistance() > 150 || getDistance() == 0)) {
        setNewState(notInUse);
      }
      break;
    case menu:
      checkButtons();
      break;
    case triggered:
      break;
  }
}
void setNewState(State newState) {
  state = newState;
  stateChanged = true;
}

void doStateTransition() {
  setLEDColor();
  setLCD();
}

void clockWatch(int frequency, unsigned long* lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    *lastRunMillis = millis();
    (*f)();
  }
}
