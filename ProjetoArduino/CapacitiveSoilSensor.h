#ifndef CAPACITIVESOILSENSOR_H
#define CAPACITIVESOILSENSOR_H

#include <Arduino.h>

class CapacitiveSoilSensor {
  public:
    // Construtor: recebe o pino analógico e os valores de calibração
    CapacitiveSoilSensor(uint8_t pin, int airValue, int waterValue);

    // Método para ler a umidade em porcentagem
    int readMoisturePercentage();
    // Método para ler o valor analógico bruto
    int readRawValue();

  private:
    uint8_t _pin;
    int _airValue;    // Valor do sensor no ar (seco) [3, 4, 5]
    int _waterValue;  // Valor do sensor na água (úmido) [3, 4, 5]
};

#endif