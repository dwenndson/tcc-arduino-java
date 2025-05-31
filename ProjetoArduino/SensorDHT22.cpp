#include "DHT22Sensor.h"

const unsigned long INTERVALO_LEITURA_DHT = 2000; // Intervalo mínimo entre leituras do DHT

SensorDHT22::SensorDHT22(uint8_t pinoDados, uint8_t tipoSensor)
    : _dht(pinoDados, tipoSensor), _tipoSensor(tipoSensor), // Inicializa _dht e _tipoSensor
      _temperaturaC(NAN), _temperaturaF(NAN), _umidadeAr(NAN),
      _ultimaLeituraSucesso(false), _tempoUltimaLeitura(0) {
}

void SensorDHT22::iniciar() {
  _dht.begin();
  Serial.println(F("Sensor DHT22 inicializado."));
}

void SensorDHT22::_realizarLeitura() {
  unsigned long agora = millis();
  // Só realiza uma nova leitura se o intervalo mínimo tiver passado
  // ou se for a primeira leitura (tempoUltimaLeitura == 0 e _ultimaLeituraSucesso == false implicitamente)
  if (agora - _tempoUltimaLeitura >= INTERVALO_LEITURA_DHT || _tempoUltimaLeitura == 0) {
    _umidadeAr = _dht.readHumidity();
    _temperaturaC = _dht.readTemperature();         // Lê em Celsius
    _temperaturaF = _dht.readTemperature(true);    // Lê em Fahrenheit

    if (isnan(_umidadeAr) || isnan(_temperaturaC) || isnan(_temperaturaF)) {
      // Serial.println(F("Falha ao ler dados do sensor DHT22!")); // Mensagem pode ser no loop principal
      _ultimaLeituraSucesso = false;
      // Mantém os valores antigos ou NAN se a primeira leitura falhar
    } else {
      _ultimaLeituraSucesso = true;
    }
    _tempoUltimaLeitura = agora;
  }
  // Se o intervalo não passou, _ultimaLeituraSucesso mantém seu valor anterior
}

bool SensorDHT22::ultimaLeituraOk() {
  _realizarLeitura(); // Garante que uma leitura seja tentada se o intervalo passou
  return _ultimaLeituraSucesso;
}

float SensorDHT22::lerTemperaturaCelsius() {
  // _realizarLeitura(); // Chamado por ultimaLeituraOk()
  return _ultimaLeituraSucesso ? _temperaturaC : NAN;
}

float SensorDHT22::lerTemperaturaFahrenheit() {
  // _realizarLeitura(); // Chamado por ultimaLeituraOk()
  return _ultimaLeituraSucesso ? _temperaturaF : NAN;
}

float SensorDHT22::lerUmidadeAr() {
  // _realizarLeitura(); // Chamado por ultimaLeituraOk()
  return _ultimaLeituraSucesso ? _umidadeAr : NAN;
}

float SensorDHT22::calcularIndiceCalorCelsius() {
  // _realizarLeitura(); // Chamado por ultimaLeituraOk()
  if (_ultimaLeituraSucesso) {
    return _dht.computeHeatIndex(_temperaturaC, _umidadeAr, false); // false para entrada em Celsius
  }
  return NAN;
}

float SensorDHT22::calcularIndiceCalorFahrenheit() {
  // _realizarLeitura(); // Chamado por ultimaLeituraOk()
  if (_ultimaLeituraSucesso) {
    return _dht.computeHeatIndex(_temperaturaF, _umidadeAr, true); // true para entrada em Fahrenheit
  }
  return NAN;
}