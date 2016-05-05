#include "sensor_agua.h"
#include <Arduino.h>

SensorAgua::SensorAgua (int pin)
{
  _pin = pin;
}

void SensorAgua::reset()
{
  pinMode(_pin, INPUT);
}

bool SensorAgua::expuesto_al_agua()
{
  return (digitalRead(_pin) == LOW);
}

void SensorAgua::serial()
{
  Serial.println((expuesto_al_agua()) ? "CON AGUA" : "SIN AGUA");
}
