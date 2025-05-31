#include "CapacitiveSoilSensor.h"

// Construtor
CapacitiveSoilSensor::CapacitiveSoilSensor(uint8_t pin, int airValue, int waterValue) {
  _pin = pin;
  _airValue = airValue;
  _waterValue = waterValue;
}

// Lê o valor analógico bruto
int CapacitiveSoilSensor::readRawValue() {
  return analogRead(_pin); // [4, 5, 6]
}

// Lê a umidade em porcentagem
int CapacitiveSoilSensor::readMoisturePercentage() {
  int rawValue = analogRead(_pin); // [4, 5, 6]

  // Mapeia o valor bruto para uma porcentagem.
  // A relação pode ser inversa: menor valor bruto = mais úmido. [4, 5]
  // Se rawValue for menor para mais úmido, então waterValue será menor que airValue.
  // map(value, fromLow, fromHigh, toLow, toHigh)
  // Se waterValue < airValue (comum em sensores capacitivos onde a tensão CAI com a umidade):
  // toLow = 100 (para úmido), toHigh = 0 (para seco)
  int percentage = map(rawValue, _waterValue, _airValue, 100, 0); // [7, 3, 4]

  // Garante que a porcentagem fique entre 0 e 100
  percentage = constrain(percentage, 0, 100); // [7, 3]
  return percentage;
}