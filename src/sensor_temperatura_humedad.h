#ifndef __SENSOR_TEMPERATURA_HUMEDAD_H__
#define __SENSOR_TEMPERATURA_HUMEDAD_H__

/**
 * ## Sensor de temperatura y humedad ##
 * 
 *   - Artículo: Grove Temperature and Humidity Sensor Pro 
 *   - Fabricante: Seeed studio
 *   - SKU: 101020019
 *
 * Medidas:
 * 
 *   - Temperatura
 *   - Humedad
 *
 * Documentación:
 * 
 *   - http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro
 *
 * Especificaciones:
 *
 *   - Voltaje: 3.3 - 6 DVC
 *   - Consumo: 1 - 1.5 mA en medida, 40 - 50 uA en espera
 *   - Humedad:
 *       - Rango: 5% - 99% RH
 *       - Precisión: +- 2% RH
 *       - Repetibilidad: +-0.3% RH
 *   - Temperatura:
 *       - Rango: 40 - 80 ºC
 *       - Precisión: +- 0.5 ºC
 *       - Repetibilidad: +- 0.2 ºC
 *   - Estabilidad a largo plazo: +- 0.5% RH/año
 *   - Tiempo de recolección de señal: 2s
 *   - Tiempo de respuesta: 6 - 20 s
 *
 * Librería:
 *
 *     - https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
 *
 * Notas:
 *
 *     - El tipo de sensor es DHT 22 (AM2302)
 *     
 */

#include <stdbool.h>
#include <Arduino.h>
#include "DHT.h"

class SensorTemperaturaHumedad : public DHT
{
 public:
  SensorTemperaturaHumedad(uint8_t pin, uint8_t type);
  
  void leer();
  void reset();
  
  void serial();
  
  float temperatura();
  float humedad();        
  bool error();
  
  
 private:
  bool _error;
  float _temperatura;
  float _humedad;
  
  const float TEMPERATURA_MINIMA = -40.0;
  const float TEMPERATURA_MAXIMA = 80.0;
  
  const float HUMEDAD_MINIMA = 5.00;
  const float HUMEDAD_MAXIMA = 99.0;
  
};

#endif
