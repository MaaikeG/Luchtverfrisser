#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14 // Temperature sensor input
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int magnet = 19;
const int trig = 16; // Distance sensor trigger
const int echo = 17; // Distance sensor echo
long duration, cm, inches;

const uint8_t debounceDelay = 50;
unsigned long lastDebounceTime;

int lastButtonStates;
int currentButtonStates; // contains current and last states of each button.
// The bit on the position that is also the index of the button contains
// the last state for that button.

void setupSensors() {
  sensors.begin();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

int getTemperature() {
  // Send the command to get temperatures
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

int getDistance() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echo, HIGH);

  // convert the time into a distance
  cm = (duration / 2) / 29.1;

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}

int getMagnetState() {
  return debouncedDigitalRead(magnet);
}

bool debouncedDigitalRead(int buttonPin) {
  uint8_t bitPosition = buttonPin / 2;
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

