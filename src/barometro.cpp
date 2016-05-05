#include "barometro.h"

Barometro::Barometro()
{
  _temperatura = 0.0;
  _presion = 0.0;	
}

void Barometro::reset()
{
  sensor.init();
  _temperatura = 0.0;
  _presion = 0.0;
}

void Barometro::leer_temperatura()
{
  float t = sensor.bmp085GetTemperature(sensor.bmp085ReadUT());
  _temperatura = constrain(t, TEMPERATURA_MINIMA, TEMPERATURA_MAXIMA);
}

void Barometro::leer_presion()
{
  float p = sensor.bmp085GetPressure(sensor.bmp085ReadUP());
  _presion = constrain(p, PRESION_MINIMA, PRESION_MAXIMA);
}

float Barometro::altitud()
{
  return sensor.calcAltitude(_presion);
}

float Barometro::atmosferas()
{
  return (_presion / 101325);
}

float Barometro::temperatura()
{
  return _temperatura;
}

float Barometro::presion()
{
  return _presion;
}

void Barometro::leer()
{
  leer_temperatura();
  leer_presion();
}

void Barometro::serial()
{
  Serial.print("Tª: ");
  Serial.print(temperatura(), 2);
  Serial.print(" ºC | P: ");
  Serial.print(presion() / 100, 0);
  Serial.print(" PA. | A: ");
  Serial.print(altitud(), 2);
  Serial.print(" M. | ATM: ");
  Serial.print(atmosferas(), 4);
  Serial.println(" ATM.");
}
