#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>

// TEMPERATURE VARIABLES
#define ONE_WIRE_BUS 14 // Temperature sensor input
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temperature;

// DISTANCE VARIABLES
#define trigger 16  // Arduino pin tied to triggerger pin on the ultrasonic sensor.
#define echo 17  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define maxDistance 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
uint8_t distance;
unsigned long lastDistanceReading; // clockwatch variable
NewPing sonar(trigger, echo, maxDistance); // NewPing setup of pins and maximum distance.

// MOTION VARIABLES
#define motion 18
unsigned long lastMotionDetected; // last time motion was detected.

// MAGNET VARIABLES
#define magnet A5
unsigned long doorLastOpen;

// DEBOUNCING VARIABLES
#define debounceDelay 50
unsigned long lastDebounceTime;

// BUTTON VARIABLES
unsigned long lastButtonStates;
unsigned long currentButtonStates; // contains current and last states of each button.
// The bit on the position that is also the index of the button contains
// the last state for that button.

#define maxLadderPosition0Voltage 50
#define minLadderPosition1Voltage 400
#define maxLadderPosition1Voltage 600

void setupSensors() {
  sensors.begin();
  sensors.setResolution(0, 9);
}

int8_t getTemperature() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  return sensors.getTempCByIndex(0);
}

uint8_t getDistance() {
  clockWatch(100, &lastDistanceReading, []() {
    distance = sonar.ping_cm();
  });
  return distance;
}

uint8_t readMotionDetector() {
  uint8_t reading = digitalRead(motion);
  if (reading == HIGH)
    lastMotionDetected = millis();
  return reading;
}

unsigned long getLastMotionDetected() {
  readMotionDetector();
  return lastMotionDetected;
}

unsigned long getDoorCloseTime() {
  return doorLastOpen;
}

unsigned long readMagnet() {
  uint8_t doorState = debouncedAnalogRead(magnet, 1);
  if (doorState == LOW) {
    doorLastOpen = millis();
  }
  return doorState;
}

bool debouncedAnalogRead(uint8_t buttonPin, uint8_t ladderPosition) {
  unsigned long reading = analogRead(buttonPin);
  int digitalReading;
  switch (ladderPosition) {
    case 0: 
      digitalReading = (reading < maxLadderPosition0Voltage) ? 1UL : 0;
      break;
    case 1:
      digitalReading = (reading > minLadderPosition1Voltage && reading < maxLadderPosition1Voltage) ? 1UL : 0;
      break;
  }
  return debounce(buttonPin + ladderPosition, digitalReading);
}

bool debouncedDigitalRead(uint8_t buttonPin) {
  return debounce(buttonPin, digitalRead(buttonPin));
}

bool debounce (uint8_t buttonPin, unsigned long reading) {
  uint8_t bitPosition = buttonPin;
  uint8_t currentState = (currentButtonStates & (1UL << bitPosition)) > 0;
  uint8_t lastState = (lastButtonStates & (1UL << bitPosition)) > 0;
  
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

