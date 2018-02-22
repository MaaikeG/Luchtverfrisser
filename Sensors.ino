#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14 // Temperature sensor input
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
unsigned long lastTempReading = 0;
int tempReadDelay = 1000;
float temperature;

const int magnet = 19;
const int trig = 16; // Distance sensor trigger
const int echo = 17; // Distance sensor echo
long duration, cm, inches;

void setupSensors() {
  sensors.begin();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

int getTemperature() {
  clockWatch(tempReadDelay, &lastTempReading, []() {
    // Send the command to get temperatures
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
  });
  return temperature;
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
  pinMode(echo, INPUT);
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

