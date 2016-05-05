# curso-arduino-nivel-ii
Trabajo realizado para el curso "Arduino avanzado nivel II" - impartido en el CEP de Linares en Mayo de 2016

# Invernadero digital

Se automatiza el cuidado de un invernadero instalando sensores de temperatura 
y humedad y actuadores de ventilación y riego.

Tras tomar la temperatura (interior y exterior) y la humedad del
suelo se toma la decisión de encender los ventiladores para
refrescar o cambiar la humedad del aire.

Tras tomar la humedad del suelo se decide si encender una bomba de
agua para regar las plantas. El mecanismo de seguridad de la bomba
requiere un sensor de agua para asegurarnos que la bomba no esté
funcionando si no hay agua en el depósito.
 
## Sensores
 
Los sensores utilizados son los siguientes:
 
- Seeedstudio Grove Temperature & humidity Pro (fichero "sensor_humedad_temperatura.h")
- Seeedstudio Grove Barometer (fichero "barometro.h")
   
## Unidades de medida
 
Las unidades de medida utilizadas por los sensores son:

- Potencial eléctrico:
  - V por Voltios
- Intensidad:
  - A por Amperios
  - uA por micro Amperios
- Humedad:
  - RH por humedad relativa (%)
- Presión:
   - PA por Pascales
- Frecuencia:
   - HZ por Herzio
   - MHZ por Mega Herzio
 - Tiempo:
   - s por segundos


# TODO

- [ ] Poner ventiladores en lugar de motor.
- [ ] Probar la escritura en tarjeta SD
- [ ] Crear la infraestructura del invernadero e instalar los sensores.
