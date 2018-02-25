#define freshener 13
//for testing purposes
#define doorDistance 75

// TODO: Save these in EEPROM!!!
uint16_t sprayDelay = 15000; // delay in ms
uint16_t spraysRemaining = 2400;

unsigned long triggeredAt;
uint8_t nSprays;

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

void setup() {
  Serial.begin(9600);
  setupLEDs();
  setupLCD();
  setupSensors();
  setupButtons();
  pinMode(freshener, OUTPUT);
  stateChanged = true;
  getTemperature();
}

void loop() {
  if (stateChanged) { // handle a state change
    doStateTransition();
    stateChanged = false;
  }
  changeLEDcolor();
  unsigned long lastMotionDetected = getLastMotionDetected();

  switch (state) {
    case notInUse:
      printTemperature();
      if (readMotionDetector() == HIGH) {
        setNewState(useUnknown);
      }
      break;
    case useUnknown:
      if (millis() - lastMotionDetected > 5000 && getDistance() > doorDistance) {
        setNewState(notInUse);
      }
      break;
    case type1Use:
    case type2Use:
      if (readMagnet() == 1 // door is closed
          && millis() - getDoorCloseTime() > 2000 // was closed 2 sec ago
          && millis() - getLastMotionDetected() > 2000) { // and no motion detected for 2 sec.
        setNewState(triggered);
      }
      break;
    case cleaning:
      if (millis() - lastMotionDetected > 5000 && getDistance() > doorDistance) {
        setNewState(notInUse);
      }
      break;
    case menu:
      checkButtons();
      break;
    case triggered:
      if (millis() - triggeredAt >= sprayDelay) {
        spray();
        setNewState(notInUse);
      }
      break;
  }
}

void setNewState(State newState) {
  if (newState == triggered) {
    triggeredAt = millis();
    nSprays = state == type2Use ? 2 : 1;
  }
  state = newState;
  stateChanged = true;
}

void doStateTransition() {
  setLEDColor();
  setLCD();
}

void spray() {
  digitalWrite(freshener, HIGH);
  spraysRemaining -= nSprays;
}

void clockWatch(int frequency, unsigned long* lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    *lastRunMillis = millis();
    (*f)();
  }
}
