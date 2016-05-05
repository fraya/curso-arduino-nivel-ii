#ifndef __SENSOR_HUMEDAD_SUELO_H__
#define __SENSOR_HUMEDAD_SUELO_H__

/**
 * ## HUMEDAD DEL SUELO ##
 *
 *  Se calcula la humedad del suelo para controlar cuando es necesario regar.
 *  
 * Grove - Moisture Sensor
 * 
 * Fabricante:
 *
 *   - Seeed Studio
 *
 * SKU:
 *
 *   - 101020008
 *
 * Documentación:
 *
 *   - http://seeedstudio.com/wiki/Grove_-_Moisture_Sensor
 *
 * Especificaciones:
 *
 *   - Voltaje: 3.3 - 5 V
 *   - Intensidad: 0 - 35 mA
 *   - Valores de salida:
 *      - Suelo seco: 0 - 300
 *      - Suelo húmedo: 300 - 700
 *      - Sensor en agua: 700 - 950
 *
 */

#include <stdbool.h>
#include <Arduino.h>

enum TipoSuelo {
  seco,
  humedo,
  encharcado,
  desconocido
};   

class SensorHumedadSuelo
{
 public:
  SensorHumedadSuelo(int pin);
        
  void autocalibrar();
  void leer();
  void reset();
  
  void serial();
  
  int salida();
  TipoSuelo tipo_suelo();                
  
 private:
  int _pin;
  int _salida;
  
  bool en_rango(const int rango[2], int valor);
  
  const int RANGO_SUELO_SECO[2] = { 0, 299 };
  const int RANGO_SUELO_HUMEDO[2] = { 300, 699 };
  const int RANGO_SUELO_ENCHARCADO[2] = { 700, 950 };
  
  const int SALIDA_MINIMA = 0;        
  const int SALIDA_MAXIMA = 950;
};

#endif
