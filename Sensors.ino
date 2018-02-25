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
#define magnet 19
unsigned long doorLastOpen;

// DEBOUNCING VARIABLES
#define debounceDelay 50
unsigned long lastDebounceTime;

// BUTTON VARIABLES
unsigned int lastButtonStates;
unsigned int currentButtonStates; // contains current and last states of each button.
// The bit on the position that is also the index of the button contains
// the last state for that button.


void setupSensors() {
  sensors.begin();
}

int getTemperature() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  return sensors.getTempCByIndex(0);
}

int getDistance() {
  clockWatch(100, &lastDistanceReading, []() {
    distance = sonar.ping_cm();
  });
  return distance;
}

int readMotionDetector() {
  int reading = digitalRead(motion);
  if (reading == HIGH)
    lastMotionDetected = millis();
  return reading;
}

int getLastMotionDetected() {
  readMotionDetector();
  return lastMotionDetected;
}

unsigned long getDoorCloseTime() {
  return doorLastOpen;
}

unsigned long readMagnet() {
  int doorState = debouncedDigitalRead(magnet);
  if (doorState == LOW) {
    doorLastOpen = millis();
  }
  return doorState;
}

bool debouncedDigitalRead(int buttonPin) {
  int bitPosition = buttonPin;
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

