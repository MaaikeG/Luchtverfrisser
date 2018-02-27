#define freshener 13
//for testing purposes
#define doorDistance 75
#define cleaningDelay 5000

// TODO: Save these in EEPROM!!!
uint16_t sprayDelay = 3000; // delay in ms
uint16_t spraysRemaining = 2400;

unsigned long triggeredAt;
unsigned long sprayInterval;
uint8_t spraying;
// number of sprays imminent
uint8_t nSprays;

// Number of times to spray for number 1 or number 2
uint8_t nSpraysUse1 = 1;
uint8_t nSpraysUse2 = 2;

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
  // i don't understand why, but these can't be in the cases below
  unsigned long lastMotionDetected = getLastMotionDetected();
  uint8_t magnetReading = readMagnet();

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
      }else if(millis() - lastMotionDetected > cleaningDelay && magnetReading == LOW){
        setNewState(cleaning);
      }
      break;
    case type1Use:
    case type2Use:
      if (readMagnet() == 1 // door is closed
          && millis() - getDoorCloseTime() > 2000 // was closed 2 sec ago
          && millis() - lastMotionDetected > 2000) { // and no motion detected for 2 sec.
        trigger(state == type1Use ? nSpraysUse1 : nSpraysUse2);
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
      printTriggerCountDown();
      if (nSprays <= 0) {
        Serial.println("Done!");
        setNewState(notInUse);
      }
      else if (millis() - triggeredAt >= sprayDelay) {
        spray();
      }
      break;
  }
}

void trigger(int _nSprays) {
  triggeredAt = millis();
  nSprays = _nSprays;
  setNewState(triggered);
}

void setNewState(State newState) {
  state = newState;
  stateChanged = true;
}

void doStateTransition() {
  setLEDColor();
  setLCD();
}

void spray() {
  clockWatch(750, &sprayInterval, []() {
    if (!spraying) {
      spraying = true;
      digitalWrite(freshener, HIGH);
    }
    else {
      digitalWrite(freshener, LOW);
      spraying = false;
      nSprays -= 1;
      spraysRemaining -= 1;
    }
  });
}

void clockWatch(int frequency, unsigned long * lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    *lastRunMillis = millis();
    (*f)();
  }
}
