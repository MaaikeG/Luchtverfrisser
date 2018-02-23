const uint8_t freshenerPin = 13;

const uint16_t maxSprayDelay = 32000;
const uint16_t startSpraysRemaining = 2400;
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



