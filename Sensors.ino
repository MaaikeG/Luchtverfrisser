#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>

#define ONE_WIRE_BUS 14 // Temperature sensor input
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define trig  16  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define echo  17  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define maxDistance 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(trig, echo, maxDistance); // NewPing setup of pins and maximum distance.

float temperature;
unsigned long lastTempReading = 0;
int tempReadDelaySeconds = 30;

const int magnet = 19;
const int motion = 18;

const uint8_t debounceDelay = 50;
unsigned long lastDebounceTime;

unsigned int lastButtonStates;
unsigned int currentButtonStates; // contains current and last states of each button.
// The bit on the position that is also the index of the button contains
// the last state for that button.

unsigned int lastMotionDetected;

void setupSensors() {
  sensors.begin();
}

int getTemperature() {
  clockWatch(tempReadDelaySeconds * 1000, &lastTempReading, []() {
    sensors.requestTemperatures(); // Send the command to get temperatures
    temperature = sensors.getTempCByIndex(0);
  });
  // Send the command to get temperatures
  return temperature;
}

int getDistance() {
  Serial.print(sonar.ping_cm());
}

int getMagnetState() {
  return debouncedDigitalRead(magnet);
}

void detectMotion() {
  if (digitalRead(motion)) {
    lastMotionDetected = millis();
  }
}

int getLastMotionDetected() {
  return lastMotionDetected;
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

