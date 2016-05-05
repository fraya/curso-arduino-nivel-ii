#ifndef __SENSOR_AGUA_H__
#define __SENSOR_AGUA_H__

/*
 * Grove - Water Sensor
 *
 * Fabricante:
 * 
 *     - Seeed Studio
 *
 * SKU:
 * 
 *     - 101020018
 *
 * Características:
 * 
 *     - Voltaje: 4.75 - 5.0 - 5.25 V
 *     - Consumo: < 20mA
 *     - Rango de temperatura 10-30 ºC
 *     - Rango de humedad (sin condensación): 10-90%
 *     
 * Código:
 * 
 *     - https://github.com/Seeed-Studio/Grove_Water_Sensor
 * 
 */

#include <stdbool.h>

class SensorAgua
{
 public:
  SensorAgua(int pin);
  
  void reset();
  void serial();
  
  bool expuesto_al_agua();
  
 private:
  int _pin;
  
};

#endif
