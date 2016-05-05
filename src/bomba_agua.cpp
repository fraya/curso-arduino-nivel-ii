#include "bomba_agua.h"

BombaAgua::BombaAgua(int pin, unsigned int tmf, unsigned int tmd)
{
  _pin = pin;
  _tiempo_maximo_funcionamiento = tmf;
  _tiempo_minimo_descanso = tmd;
  _tiempo_funcionamiento = 0;
  _tiempo_descanso = 0;
  _estado = Apagado;
}

void BombaAgua::inicializar()
{
  pinMode(_pin, OUTPUT);
}

void BombaAgua::set_tiempo_maximo_funcionamiento(unsigned int limite)
{
  _tiempo_maximo_funcionamiento = constrain(limite, 1, TIEMPO_MAXIMO_FUNCIONAMIENTO);
}

void BombaAgua::set_tiempo_minimo_descanso(unsigned int limite)
{
  _tiempo_minimo_descanso = limite;
}

unsigned int BombaAgua::tiempo_maximo_funcionamiento()
{
  return _tiempo_maximo_funcionamiento;
}

unsigned int BombaAgua::tiempo_minimo_descanso()
{
  return _tiempo_minimo_descanso;
}

void BombaAgua::encender()
{
  digitalWrite(_pin, HIGH);
  _tiempo_funcionamiento = millis();
  _tiempo_descanso = 0;
  _estado = Encendido;
}

void BombaAgua::apagar()
{
  digitalWrite(_pin, LOW);
  _tiempo_funcionamiento = 0;
  _tiempo_descanso = millis();
  _estado = Apagado;
}

bool BombaAgua::encendido()
{
  return _estado == Encendido;
}

bool BombaAgua::apagado()
{
  return _estado == Apagado;
}

unsigned long BombaAgua::tiempoFuncionamiento()
{
  unsigned long ahora = millis();
  return (ahora - _tiempo_funcionamiento) / 1000;
}

unsigned long BombaAgua::tiempo_descanso()
{
  unsigned long ahora = millis();
  return (ahora - _tiempo_descanso) / 1000;
}

unsigned long BombaAgua::tiempoRestanteFuncionamiento()
{
  if (tiempoFuncionamiento() > tiempo_maximo_funcionamiento()) {
    return 0;
  } else {
    return tiempo_maximo_funcionamiento() - tiempoFuncionamiento();
  }
}

unsigned long BombaAgua::tiempoRestanteDescanso()
{
  if (tiempo_descanso() > tiempo_minimo_descanso()) {
    return 0;
  } else {
    return tiempo_minimo_descanso() - tiempo_descanso();
  }	
}

bool BombaAgua::sobrecalentado()
{
  return _estado == Sobrecalentado;
}

void BombaAgua::descansar()
{
  apagar();
  _estado = Sobrecalentado;
}

void BombaAgua::preparado()
{
  _estado = Apagado;
}

bool BombaAgua::tiempoFuncionamientoExcedido()
{
  return tiempoFuncionamiento() > tiempo_maximo_funcionamiento();
}

bool BombaAgua::haDescansado()
{
  return tiempo_descanso() > tiempo_minimo_descanso();
}

void BombaAgua::serial()
{
  switch (_estado) {
  case Apagado:
    Serial.print("APAGADA ");
    break;
  case Encendido:
    Serial.print("ENCENDIDA ");
    break;
  case Sobrecalentado:
    Serial.print("SOBRECALENTADA ");
    break;
  }
  
  Serial.print("(");
  Serial.print((apagado() || sobrecalentado()) ? tiempo_descanso() : tiempoFuncionamiento());
  Serial.print(" s.");
  
  switch (_estado) {
  case Apagado:
    Serial.println(")");
    break;
  case Sobrecalentado:
    Serial.print(" / ");
    Serial.print(tiempoRestanteDescanso());
    Serial.println(" s. restantes)");
    break;
  case Encendido:
    Serial.print(" / ");
    Serial.print(tiempoRestanteFuncionamiento());
    Serial.println(" s. restantes)");
    break;
  }	
}
