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
enum MenuItems {
  sprayDelayMenu,
  spraysRemainingMenu,
  exitMenu
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
  if(stateChanged) { // handle a state change
    doStateTransition();
    stateChanged = false;
  }
   switch (state) {
    case notInUse:
      printTemperature();
      changeLEDcolor();
    case menu:
      checkButtons();
      changeLEDcolor();
      break;
    case triggered:
      changeLEDcolor();
      break;
  }
}

void doStateTransition() {
  setLEDColor();
  setLCD();
  if (state != menu) {
    attachInterrupts();
  }
  else {
    detachInterrupts();
  }
}

void clockWatch(int frequency, unsigned long* lastRunMillis, void (*f)()) {
  if (millis() - *lastRunMillis >= frequency) {
    *lastRunMillis = millis();
    (*f)();
  }
}
