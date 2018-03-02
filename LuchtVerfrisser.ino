#include <EEPROM.h>

#define freshener 13

//for testing purposes
#define doorDistance 75

#define cleaningDelay 30000

#define notInUseDelay 5000

#define maxType1Distance 90 
#define type1Delay 5000

#define maxType2Distance 50 
#define type2Delay 25000

uint16_t sprayDelay = 3000; // delay in ms
#define freshenerOnTime 1000

#define spraysRemainingAddress  0
#define emptyEepromValue 65535
#define startSpraysRemaining 2400
uint16_t spraysRemaining;

unsigned long stateTransitionAt;
unsigned long sprayInterval;
uint8_t spraying;
// number of sprays imminent
uint8_t nSprays;

// Number of times to spray for number 1 or number 2
uint8_t nSpraysUse1 = 1;
uint8_t nSpraysUse2 = 2;
uint8_t nSpraysOverride = 1;

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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  EEPROM.get(spraysRemainingAddress, spraysRemaining);
  if(spraysRemaining == emptyEepromValue){
    resetSpraysRemaining();
  }
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
  getDistance();

  if (getDistance() > maxType1Distance) {
     enteredType1Distance = millis();
  }
  if (getDistance() > maxType2Distance) {
      enteredType2Distance = millis();
  }
      
  switch (state) {
    case notInUse:
      printTemperature();
      if (readMotionDetector() == HIGH) {
        setNewState(useUnknown);
      }
      break;
    case useUnknown:
      if (millis() - lastMotionDetected > notInUseDelay && getDistance() > doorDistance) {
        setNewState(notInUse);
      } else if (millis() - enteredType1Distance > type1Delay && magnetReading == HIGH) {
        setNewState(type1Use);
      } else if (millis() - stateTransitionAt > cleaningDelay && magnetReading == LOW) {
        setNewState(cleaning);
      }
      break;
    case type1Use:
      if (millis() - enteredType2Distance > type2Delay && magnetReading == HIGH) {
        setNewState(type2Use);
      }
    case type2Use:
      if (magnetReading == LOW) {
        doorOpenSinceStateChange = true;
      }
      if (doorOpenSinceStateChange //door has been opened
          && readMagnet() == HIGH // door is now closed again
          && millis() - getDoorCloseTime() > 2000 // was closed 2 sec ago
          && millis() - lastMotionDetected > 2000) { // and no motion detected for 2 sec.
        trigger(state == type1Use ? nSpraysUse1 : nSpraysUse2);
      }
      break;
    case cleaning:
      if (millis() - lastMotionDetected > notInUseDelay && getDistance() > doorDistance) {
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
      else if (millis() - stateTransitionAt >= sprayDelay) {
        spray();
      }
      break;
  }
}

void trigger(int _nSprays) {
  nSprays = _nSprays;
  setNewState(triggered);
  doorOpenSinceStateChange = false;
}

void setNewState(State newState) {
  state = newState;
  stateChanged = true;
  stateTransitionAt = millis();
}

void doStateTransition() {
  setLEDColor();
  setLCD();
}

void spray() {
  clockWatch(freshenerOnTime, &sprayInterval, []() {
    if (!spraying) {
      spraying = true;
      digitalWrite(freshener, HIGH);
    }
    else {
      digitalWrite(freshener, LOW);
      spraying = false;
      nSprays -= 1;
      setSpraysRemaining(spraysRemaining - 1);
    }
  });
}

void resetSpraysRemaining() {
  setSpraysRemaining(startSpraysRemaining);
}

void setSpraysRemaining(uint16_t newSpraysRemaining){
  spraysRemaining = newSpraysRemaining;
  EEPROM.put(spraysRemainingAddress, spraysRemaining);
}

void clockWatch(int frequency, unsigned long * lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    *lastRunMillis = millis();
    (*f)();
  }
}
