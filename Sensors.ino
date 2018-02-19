#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setupSensors() {
    sensors.begin();
}

String getTemperatureString() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  return String(sensors.getTempCByIndex(0));
}

