#include "sensor_humedad_suelo.h"

SensorHumedadSuelo::SensorHumedadSuelo (int pin)
{
  _pin = pin;
}

void SensorHumedadSuelo::reset()
{
  _salida = SALIDA_MINIMA;
}

void SensorHumedadSuelo::leer()
{
  int h = analogRead(_pin);
  _salida = map(h, 0, 1023, SALIDA_MINIMA, SALIDA_MAXIMA);
  
}

int SensorHumedadSuelo::salida()
{
  return _salida;
}

void SensorHumedadSuelo::autocalibrar()
{
  
}

bool SensorHumedadSuelo::en_rango(const int rango[2], int valor)
{
  return (rango[0] <= valor) && (valor <= rango[1]);
}

TipoSuelo SensorHumedadSuelo::tipo_suelo()
{
  if (en_rango(RANGO_SUELO_SECO, _salida)) {
    return seco;
  }
  
  if (en_rango(RANGO_SUELO_HUMEDO, _salida)) {
    return humedo;
  }
  
  if (en_rango(RANGO_SUELO_ENCHARCADO, _salida)) {
    return encharcado;
  }
  
  return desconocido;    
}

void SensorHumedadSuelo::serial()
{
  Serial.print("Lectura: ");
  Serial.print(salida());
  Serial.print(" Tipo: ");
  switch (tipo_suelo()) {
  case seco:
    Serial.println("SECO");
    break;
  case humedo:
    Serial.println("HUMEDO");
    break;
  case encharcado:
    Serial.println("ENCHARCADO");
    break;
  case desconocido:
    Serial.println("FUERA DE RANGO");
    break;
  }
}
