#ifndef SENSORDHT22_H
#define SENSORDHT22_H

#include <Arduino.h>
#include "DHT.h"

class SensorDHT22 {
  public:
    SensorDHT22(uint8_t pinoDados, uint8_t tipoSensor);
    void iniciar();
    bool ultimaLeituraOk(); // Verifica e realiza a leitura se necessário
    float lerTemperaturaCelsius();
    float lerTemperaturaFahrenheit();
    float lerUmidadeAr();
    float calcularIndiceCalorCelsius();
    float calcularIndiceCalorFahrenheit();

  private:
    DHT _dht;
    uint8_t _tipoSensor; // Adicionado para uso no construtor do _dht
    float _temperaturaC;
    float _temperaturaF;
    float _umidadeAr;
    bool _ultimaLeituraSucesso;
    unsigned long _tempoUltimaLeitura;

    void _realizarLeitura(); // Método privado para centralizar a leitura
};

#endif