#ifndef __BAROMETRO_H
#define __BAROMETRO_H

/**
 *
 * ## Sensor barómetro ##
 *
 *  - Artículo : Grove - Barometer Sensor (BMP180)
 *  - Fabricante: Seeed studio
 *  - SKU: 
 *
 * Medidas:
 * 
 *   - Temperatura
 *   - Presión
 *   - Altitud (calculada)
 *   - Atmosferas (calculada)
 * 
 * Documentación:
 * 
 *   - http://www.seeedstudio.com/wiki/Grove_-_Barometer_Sensor_%28BMP180%29
 *
 * Especificaciones:
 *
 *   - Voltaje 3-5.5 VDC (Típico 5V)
 *   - Consumo 1.1 - 20 uA
 *   - Rango de presión 300 - 1100 hPa
 *   - Rango de temperatura: -40 a 85 ºC +-2ºC
 *   - Transferencia datos I2C hasta 3.4 MHZ
 *
 *  Librería:
 *  
 *    - https://github.com/Seeed-Studio/Grove_Barometer_Sensor
 */

#include <stdbool.h>
#include <Wire.h>
#include "Barometer.h"   

class Barometro
{
public:
  
  Barometro();
  
  // Comandos
  
  void reset();
  void leer();
  
  void serial();
  
  // Consultas
  
  float altitud();
  float atmosferas();
  float temperatura();
  float presion();
  
 private:
  Barometer sensor; // Librería del fabricante
  
  void leer_temperatura();
  void leer_presion();
  
  float _temperatura;
  float _presion;
  
  const float PRESION_MINIMA = 30000.0;   // En PA
  const float PRESION_MAXIMA = 110000.0;  // En PA
  
  const float TEMPERATURA_MINIMA = -40.0; // ºC
  const float TEMPERATURA_MAXIMA =  85.0; // ºC
};

#endif
