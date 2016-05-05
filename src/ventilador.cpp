#include "ventilador.h"
#include <Arduino.h>

Ventilador::Ventilador (int pin)
{
  _pin = pin;
  _encendido = false;
}

void Ventilador::reset()
{
  pinMode(_pin, OUTPUT);
}

void Ventilador::encender()
{
  digitalWrite(_pin, HIGH);
  _encendido = true;
}

void Ventilador::apagar()
{
  digitalWrite(_pin, LOW);
  _encendido = false;
}

bool Ventilador::encendido()
{
  return _encendido;
}

bool Ventilador::apagado()
{
  return !encendido();
}

void Ventilador::serial()
{
  if (encendido()) {
    Serial.println("ENCENDIDO");
  } else {
    Serial.println("APAGADO");
  }
}
