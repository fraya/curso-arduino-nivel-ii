#include "actuador.h"

Actuador::Actuador(int pin,
		   unsigned long tiempoMaximoFuncionamiento,
		   unsigned long tiempoMinimoDescanso)
{
  _pin = pin;
  _tiempoMaximoFuncionamiento = tiempoMaximoFuncionamiento;
  _tiempoMinimoDescanso = tiempoMinimoDescanso;
  _tiempoDescanso = 0;
  _tiempoFuncionamiento = 0;
  _estado = Apagado;
}

//////////////////////////////////////////////////////////////////////
//
// Comandos
//
//////////////////////////////////////////////////////////////////////

void Actuador::inicializar()
{
  pinMode(_pin, OUTPUT);
  apagar();
}

void Actuador::encender()
{
  digitalWrite(_pin, HIGH);
  _tiempoFuncionamiento = millis();
  _tiempoDescanso = 0;
  _estado = Encendido;
}

void Actuador::apagar()
{
  digitalWrite(_pin, LOW);
  _tiempoFuncionamiento = 0;
  _tiempoDescanso = millis();
  _estado = Apagado;
}

void Actuador::descansar()
{
  apagar();
  _estado = Sobrecalentado;
}

void Actuador::preparado()
{
  _estado = Apagado;
}

//////////////////////////////////////////////////////////////////////
//
// Consultas
//
//////////////////////////////////////////////////////////////////////

EstadoActuador Actuador::estado()
{
  return _estado;
}

bool Actuador::apagado()
{
  return _estado == Apagado;
}

bool Actuador::encendido()
{
  return _estado == Encendido;
}

bool Actuador::sobrecalentado()
{
  return _estado == Sobrecalentado;
}

bool Actuador::haDescansado()
{
  return tiempoDescanso() > tiempoMinimoDescanso();
}

bool Actuador::tiempoFuncionamientoExcedido()
{
  return tiempoFuncionamiento() > tiempoMaximoFuncionamiento();
}

unsigned long Actuador::tiempoMaximoFuncionamiento()
{
  return _tiempoMaximoFuncionamiento;
}

unsigned long Actuador::tiempoMinimoDescanso()
{
  return _tiempoMinimoDescanso;
}

unsigned long Actuador::tiempoFuncionamiento()
{
  unsigned long ahora = millis();
  return (ahora - tiempoFuncionamiento()) / 1000;
}

unsigned long Actuador::tiempoDescanso()
{
  unsigned long ahora = millis();
  return (ahora - tiempoDescanso()) / 1000;
}

unsigned long Actuador::tiempoRestanteFuncionamiento()
{
  if (tiempoFuncionamiento() > tiempoMaximoFuncionamiento()) {
    return 0;
  } else {
    return tiempoMaximoFuncionamiento() - tiempoFuncionamiento();
  }
}

unsigned long Actuador::tiempoRestanteDescanso()
{
  if (tiempoDescanso() > tiempoMinimoDescanso()) {
    return 0;
  } else {
    return tiempoMinimoDescanso() - tiempoDescanso();
  }
}

//////////////////////////////////////////////////////////////////////
//
// Configuración
//
//////////////////////////////////////////////////////////////////////

void Actuador::establecerTiempoMaximoFuncionamiento(unsigned long t)
{
  // assert positivo
  _tiempoMaximoFuncionamiento = t;
}

void Actuador::establecerTiempoMinimoDescanso(unsigned long t)
{
  _tiempoMinimoDescanso = t;
}


//////////////////////////////////////////////////////////////////////
//
// I/O
//
//////////////////////////////////////////////////////////////////////

void Actuador::serial()
{
  Serial.print("Estado: ");
  switch (_estado) {
  case Apagado:
    Serial.print("APAGADO ");
    break;
  case Encendido:
    Serial.print("ENCENDIDO ");
    break;
  case Sobrecalentado:
    Serial.print("SOBRECALENTADO ");
    break;
  }
  
  Serial.print("(");
  Serial.print((apagado() || sobrecalentado()) ? tiempoDescanso() : tiempoFuncionamiento());
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
