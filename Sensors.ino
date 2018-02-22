#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14 // Temperature sensor input
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int trig = 16; // Distance sensor trigger
const int echo = 18; // Distance sensor echo
long duration, cm, inches;

void setupSensors() {
    sensors.begin();
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

String getTemperatureString() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  return String(sensors.getTempCByIndex(0));
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
  cm = (duration/2) / 29.1;
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}

