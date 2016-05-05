#ifndef __ACTUADOR_H__
#define __ACTUADOR_H__

#include <stdbool.h>
#include <Arduino.h>
#include "estado_actuador.h"

class Actuador
{
 public:
  Actuador(int pin,
	   unsigned long tiempoMaximoFuncionamiento,
	   unsigned long tiempoMinimoDescanso);

  // comandos
  
  void inicializar();
  void encender();
  void apagar();
  void descansar();
  void preparado();
  
  // consultas
  
  EstadoActuador estado();
  bool apagado();
  bool encendido();
  bool sobrecalentado();
  bool haDescansado();  
  bool tiempoFuncionamientoExcedido();
  
  unsigned long tiempoFuncionamiento();
  unsigned long tiempoDescanso();
  unsigned long tiempoMaximoFuncionamiento();
  unsigned long tiempoMinimoDescanso();
  unsigned long tiempoRestanteFuncionamiento();
  unsigned long tiempoRestanteDescanso();
  
  // configuración
  
  void establecerTiempoMaximoFuncionamiento(unsigned long t);
  void establecerTiempoMinimoDescanso(unsigned long t);
  
  // I/O
  void serial();
	
 private:
  int _pin;
  EstadoActuador _estado;
  unsigned long _tiempoMaximoFuncionamiento;
  unsigned long _tiempoMinimoDescanso;
  unsigned long _tiempoFuncionamiento;
  unsigned long _tiempoDescanso;	
};

#endif
