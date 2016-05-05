/** \file invernadero.ino
 *  \brief Código principal del proyecto
 *  \author Fernando Raya Díaz
 *
 */

#include <stdbool.h>

// Sensores

#include "barometro.h"
#include "sensor_agua.h"
#include "sensor_temperatura_humedad.h"
#include "bomba_agua.h"
#include "sensor_humedad_suelo.h"
#include "ventilador.h"

// Tarjeta SD

#include <SPI.h>
#include <SD.h>

// Reloj tiempo real
//#include "RTClib.h"

#include "Time.h"

const unsigned long TIEMPO_MAX_FUNCIONAMIENTO = 30; // tº máximo en segundos
const unsigned long TIEMPO_MIN_DESCANSO = 15;       // tº mínimo segundos
const float TEMPERATURA_MAXIMA_INVERNADERO = 25.0;  // En ºC
const float TEMPERATURA_MINIMA_INVERNADERO = 18.0;  // En ºC
const float HUMEDAD_MAXIMA_INVERNADERO = 56.0;      // %RH
const float HUMEDAD_MINIMA_INVERNADERO = 20.0;      // %RH
const int PIN_VENTILADOR_ENTRADA = 7;
const int PIN_VENTILADOR_SALIDA = 8;
const int PIN_BOMBA_AGUA = 6;
const int PIN_SENSOR_AGUA = 2;
const int PIN_SD = 4;

enum RangoTemperatura {
  TemperaturaBaja,
  TemperaturaCorrecta,
  TemperaturaAlta
};

enum RangoHumedad {
  HumedadBaja,
  HumedadCorrecta,
  HumedadAlta
};

//////////////////////////////////////////////////////////////////////
//
// Variables
//
//////////////////////////////////////////////////////////////////////

SensorHumedadSuelo       sensor_humedad_suelo(A0);
SensorTemperaturaHumedad dht(A1, DHT22); 
Barometro                barometro;
SensorAgua               sensor_agua(PIN_SENSOR_AGUA);
BombaAgua                bomba(PIN_BOMBA_AGUA, TIEMPO_MAX_FUNCIONAMIENTO, TIEMPO_MIN_DESCANSO);
Ventilador               ventilador_entrada(PIN_VENTILADOR_ENTRADA);
Ventilador               ventilador_salida(PIN_VENTILADOR_SALIDA);
RangoTemperatura         temperatura;
RangoHumedad             humedad;
bool                     csv_ok = false;
float                    espera = 2750;

/**
 * Funciones de escritura de datos en fichero log
 */


void controlar_riego()
{	
  if (sensor_agua.expuesto_al_agua() == false) {
    Serial.println("*** ALARMA: RELLENAR DEPÓSITO DE AGUA ***");
    if (bomba.encendido()) {
      Serial.println("> ACCIÓN > APAGAR BOMBA DE AGUA");
      bomba.apagar();
    }
    return;
  }
  
  if (bomba.encendido()) {
    if (bomba.tiempoFuncionamientoExcedido()) {
      Serial.println("> ALARMA > SOBRECALENTAMIENTO BOMBA AGUA");
      Serial.println("> ACCIÓN > APAGAR BOMBA DE AGUA");
      bomba.descansar();
      return;
    }
  } else {
    if (bomba.sobrecalentado()) {
      if (bomba.haDescansado()) {
	bomba.preparado();
	Serial.println("> ACCIÓN > BOMBA PREPARADA");
      } else {
	Serial.print("> ACCIÓN > BOMBA NECESITA ENFRIARSE AÚN ");
	Serial.print(bomba.tiempoRestanteDescanso());
	Serial.println(" s.");
	return;
      }
    } 
  }
  
  switch (sensor_humedad_suelo.tipo_suelo()) {
  case seco:
    Serial.println("> ACCIÓN > SUELO SECO > REGAR ");
    if (bomba.apagado()) {
      Serial.println("> ACCIÓN > ENCENDER BOMBA DE AGUA");
      bomba.encender();
    } else {
      Serial.print("> BOMBA REGANDO: ");
      Serial.print(bomba.tiempoFuncionamiento());
      Serial.println(" segundos");
    }
    break;
  case humedo:
  case encharcado:
    Serial.println("> ACCIÓN > SUELO HUMEDO O ENCHARCADO > NO REGAR ");
    if (bomba.encendido()) {
      Serial.println("> ACCIÓN > APAGAR BOMBA DE AGUA");
      bomba.apagar();
    } 
    break;
  case desconocido:
    Serial.println("*** ALARMA: SENSOR DE HUMEDAD DE SUELO AVERIADO ***");
    break;
  }
}

void establecer_rangos_temperatura_humedad()
{
  if (dht.temperatura() < TEMPERATURA_MINIMA_INVERNADERO) {
    temperatura = TemperaturaBaja;
  } else {
    if (dht.temperatura() > TEMPERATURA_MAXIMA_INVERNADERO) {
      temperatura = TemperaturaAlta;
    } else {
      temperatura = TemperaturaCorrecta;
    }
  }
  
  if (dht.humedad() < HUMEDAD_MINIMA_INVERNADERO) {
    humedad = HumedadBaja;
  } else {
    if (dht.temperatura() > HUMEDAD_MAXIMA_INVERNADERO) {
      humedad = HumedadAlta;
    } else {
      humedad = HumedadCorrecta;
    }
  }
}

void controlar_temperatura_humedad()
{
  if (temperatura == TemperaturaBaja) {
    Serial.println("> ACCIÓN > Tª POR DEBAJO DE LÍMITE");
    Serial.println("> ACCIÓN > ¿ENCENDER CALEFACCIÓN?");
  }
  
  if (humedad == HumedadBaja) {
    // ? Regar?
    Serial.println("> ACCIÓN > HUMEDAD POR DEBAJO DE MÍNIMO");
    Serial.println("> ACCIÓN > ¿REGAR?");
  }
  
  if (temperatura == TemperaturaAlta || humedad == HumedadAlta) {
    Serial.println("> ACCIÓN > Tª o HUMEDAD POR ENCIMA DE LÍMITE");
    
    if (ventilador_entrada.apagado()) {
      Serial.println("> ACCIÓN > ENCENDER VENTILADOR ENTRADA");
      ventilador_entrada.encender();
    }
    
    if (ventilador_salida.apagado()) {
      Serial.println("> ACCIÓN > ENCENDER VENTILADOR SALIDA");
      ventilador_salida.encender();
    }
  }
  
  if (temperatura != TemperaturaAlta && humedad != HumedadAlta) {
    Serial.println("> ACCIÓN > Tª y HUMEDAD EN RANGO CORRECTO");
    
    if (ventilador_entrada.encendido()) {
      Serial.println("> ACCIÓN > APAGAR VENTILADOR ENTRADA"); 
      ventilador_entrada.apagar();
    }
    
    if (ventilador_salida.encendido()) {
      Serial.println("> ACCIÓN > APAGAR VENTILADOR SALIDA");
      ventilador_salida.apagar();
    }
  }
}

void controlar_humedad()
{
  if (dht.humedad() < HUMEDAD_MINIMA_INVERNADERO) {
    // ? Regar?
    Serial.println("> ACCIÓN > HUMEDAD POR DEBAJO DE MÍNIMO");
    return;
  }
  
  if (dht.humedad() > HUMEDAD_MAXIMA_INVERNADERO) {
    Serial.println("> ACCIÓN > HUMEDAD POR ENCIMA DE MÁXIMO");
    
    if (ventilador_entrada.apagado()) {
      Serial.println("> ACCIÓN > ENCENDER VENTILADOR ENTRADA"); 
      ventilador_entrada.encender();
    }
    
    if (ventilador_salida.apagado()) {
      Serial.println("> ACCIÓN > ENCENDER VENTILADOR SALIDA");
      ventilador_salida.encender();
    }
    
    return;
  }
  
  // Humedad en rango
  
}

/**
 * Escribe en un fichero CSV de la tarjeta SD los 
 * datos de los sensores.
 * 
 * @returns `true` si se pudo escribir, `false` si hubo
 *          algún problema al escribir.
 */
bool csv_log(File csv)
{
  byte datos_escritos = csv.print(barometro.temperatura(),2);
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(",");
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(barometro.presion() / 100,2);
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(",");
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(dht.temperatura(),2);
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(",");
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(dht.humedad(),2);
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(",");
  if (datos_escritos <= 0) return false;
  datos_escritos = csv.print(sensor_humedad_suelo.salida(),2);
  if (datos_escritos <= 0) return false;
  csv.close();

  return true;
}


//////////////////////////////////////////////////////////////////////
//
// SETUP
//
//////////////////////////////////////////////////////////////////////

void setup()
{
  // Esperar a que se estabilice la corriente
  delay(1000);
  
  // Sensores
	
  barometro.reset();
  dht.reset();
  sensor_agua.reset();
  sensor_humedad_suelo.reset();
  
  // Actuadores
  bomba.inicializar();	
  ventilador_entrada.reset();
  ventilador_salida.reset();
  
  // RTC
  //RTC.begin();
  // Comentar siguiente línea una vez establecida la fecha y hora
  //RTC.adjust(DateTime(__DATE__,__TIME__)); 
  
  // Serial
  
  Serial.begin(9600);
  Serial.println("\nARDUINO ARRANCADO\n");
  
  // SD
  
  Serial.println("Inicializando tarjeta SD");
  if (SD.begin(PIN_SD)) {
    Serial.println("Tarjeta SD inicializada.");
    csv_ok = true;
  } else {
    Serial.println("No se pudo inicializar la tarjeta SD");
    csv_ok = false;
  }
  
}

//////////////////////////////////////////////////////////////////////
//
// LOOP
//
//////////////////////////////////////////////////////////////////////

void loop()
{
  delay(espera);
  
  barometro.leer();
  dht.leer();
  sensor_humedad_suelo.leer();
  establecer_rangos_temperatura_humedad();
  
  delay(espera);
  
  // Mostrar información en pantalla
  
  Serial.print("EXTERIOR  => "); barometro.serial();	
  Serial.print("INTERIOR  => "); dht.serial();	
  Serial.print("SUELO     => "); sensor_humedad_suelo.serial();
  Serial.print("DEP. AGUA => "); sensor_agua.serial();

  // Grabar información de sensores en tarjeta SD
  // Al no usar RTC, si se reinicia añadiría datos a
  // ficheros ya escritos
  
  if (csv_ok) {
    
    String filename = "";
    filename += year();
    filename += "-";
    filename += month();
    filename += "-";
    filename += day();
    filename += "-";
    filename += hour();
    filename += ".csv";
    
    File csv = SD.open(filename.c_str(), FILE_WRITE);
    
    if (!csv) {
      Serial.print("No se pudo escribir en el fichero: ");
      Serial.println(filename);
      csv_ok = false;
    } else {
      csv_ok = csv_log(csv);
    }
  }
  
  switch(temperatura) {
  case TemperaturaAlta:
    Serial.println("Tª ALTA");
    break;
  case TemperaturaBaja:
    Serial.println("Tª BAJA");
    break;
  case TemperaturaCorrecta:
    Serial.println("Tª EN RANGO");
  }
  
  switch(humedad) {
  case HumedadAlta:
    Serial.println("HUMEDAD ALTA");
    break;
  case HumedadBaja:
    Serial.println("HUMEDAD BAJA");
    break;
  case HumedadCorrecta:
    Serial.println("HUMEDAD EN RANGO");
    break;
  }
  
  
  Serial.print("BOMBA     => "); bomba.serial();	
  Serial.print("V.ENTRADA => "); ventilador_entrada.serial();
  Serial.print("V.SALIDA  => "); ventilador_salida.serial();
  
  
  controlar_riego();
  controlar_temperatura_humedad();  
}