#ifndef __VENTILADOR_H__
#define __VENTILADOR_H__

#include <stdbool.h>

class Ventilador
{
 public:
  Ventilador(int pin);
  
  void reset();
  void encender();
  void serial();
  
  void apagar();
  bool encendido();
  bool apagado();
  
 private:
  int _pin;
  bool _encendido;
  
};

#endif
