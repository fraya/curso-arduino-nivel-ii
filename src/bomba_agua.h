#ifndef __BOMBA_AGUA_H__
#define __BOMBA_AGUA_H__

#include <stdbool.h>
#include <Arduino.h>
#include "estado_actuador.h"

/**
 * La bomba de riego tiene un tiempo máximo de funcionamiento de 15
 * minutos. Hay que parar la bomba antes de que llegue a este límite,
 * por ello se establece el máximo por debajo de este máximo.
 *
 * El tiempo se calcula en segundos.        
 */

class BombaAgua
{
 public:
  BombaAgua(int pin, unsigned int tmf, unsigned int tmd);
  
  void inicializar();
  void encender();
  void apagar();
  
  void set_tiempo_maximo_funcionamiento(unsigned int limite);	
  void set_tiempo_minimo_descanso(unsigned int limite);
  
  bool apagado();
  bool encendido();
  bool sobrecalentado();
  void serial();
  
  unsigned int tiempo_maximo_funcionamiento();
  unsigned int tiempo_minimo_descanso();
  
  unsigned long tiempoFuncionamiento();
  unsigned long tiempo_descanso();
  
  unsigned long tiempoRestanteFuncionamiento();
  unsigned long tiempoRestanteDescanso();
  
  bool tiempoFuncionamientoExcedido();
  
  void descansar();
  void preparado();
  bool haDescansado();
  
 private:
  int _pin;
  EstadoActuador _estado;
  
  unsigned int _tiempo_maximo_funcionamiento;
  unsigned int _tiempo_minimo_descanso;
  
  unsigned long _tiempo_funcionamiento;
  unsigned long _tiempo_descanso;
  
  const unsigned int TIEMPO_MAXIMO_FUNCIONAMIENTO = 14 * 60;
  const unsigned int TIEMPO_MINIMO_DESCANSO = 1 * 60;
  
};

#endif
