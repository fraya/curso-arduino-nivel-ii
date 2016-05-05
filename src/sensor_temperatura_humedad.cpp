#include "sensor_temperatura_humedad.h"

SensorTemperaturaHumedad::SensorTemperaturaHumedad(uint8_t pin, uint8_t type) :
  DHT(pin, type)
{
  _temperatura = TEMPERATURA_MINIMA;
  _humedad = HUMEDAD_MINIMA;
  _error = false;
}

void SensorTemperaturaHumedad::reset()
{
  begin();
  _temperatura = TEMPERATURA_MINIMA;
  _humedad = HUMEDAD_MINIMA;
  _error = false;
}

void SensorTemperaturaHumedad::leer()
{
  float t = readTemperature();
  if (isnan(t)) {
    _error = true;
    _temperatura = t;
  } else {
    _temperatura = constrain(t, TEMPERATURA_MINIMA, TEMPERATURA_MAXIMA);
  }
  
  float h = readHumidity();
  if (isnan(h)) {
    _error = true;
    _humedad = t;
  } else {
    _humedad = constrain(h, HUMEDAD_MINIMA, HUMEDAD_MAXIMA);
  }
}

float SensorTemperaturaHumedad::temperatura()
{
  return _temperatura;
}

float SensorTemperaturaHumedad::humedad()
{
  return _humedad;
}


void SensorTemperaturaHumedad::serial()
{
  Serial.print("Tª: ");
  Serial.print(temperatura(), 2);
  Serial.print(" ºC | ");
  Serial.print("H: ");
  Serial.print(humedad(), 1);
  Serial.println(" \%RH");
}
