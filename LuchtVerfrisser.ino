#define freshener 13
//for testing purposes
#define doorDistance 75
#define cleaningDelay 20000
#define minType1Distance 40
#define maxType1Distance 70
#define type1Delay 3000
#define minType2Distance 10
#define maxType2Distance 30
#define type2Delay 5000

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

unsigned long enteredType1Distance;
unsigned long enteredType2Distance;

uint8_t doorOpenSinceStateChange;

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
      if ((getDistance() < minType1Distance || getDistance() > maxType1Distance) && getDistance() != 0) {
        enteredType1Distance = millis();
      }
      if ((getDistance() < minType2Distance || getDistance() > maxType2Distance) && getDistance() != 0) {
        enteredType2Distance = millis();
      }
      if (millis() - lastMotionDetected > 5000 && getDistance() > doorDistance) {
        setNewState(notInUse);
      } else if (millis() - enteredType1Distance > type1Delay && magnetReading == HIGH) {
        setNewState(type1Use);
      } else if (millis() - enteredType2Distance > type2Delay && magnetReading == HIGH) {
        setNewState(type2Use);
      } else if (millis() - lastMotionDetected > cleaningDelay && magnetReading == LOW) {
        setNewState(cleaning);
      }
      break;
    case type1Use:
    case type2Use:
      if (readMagnet() == 0) {
        doorOpenSinceStateChange = true;
      }
      if (doorOpenSinceStateChange //door has been opened
          && readMagnet() == 1 // door is now closed again
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
  doorOpenSinceStateChange = false;
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
