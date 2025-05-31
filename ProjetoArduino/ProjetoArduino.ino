#include "CapacitiveSoilSensor.h"
#include "DHT22Sensor.h"
// --- CONFIGURAÇÕES GLOBAIS E OBJETOS DOS SENSORES ---
const int PINO_DHT22 = 2;
const int PINO_UMIDADE_SOLO = A0;

// IMPORTANTE: CALIBRAR ESTES VALORES!
const int VALOR_SOLO_SECO_AR = 700;
const int VALOR_SOLO_AGUA = 300;

SensorDHT22 dhtSensor(PINO_DHT22, DHT22);
CapacitiveSoilSensor soloSensor(PINO_UMIDADE_SOLO, VALOR_SOLO_SECO_AR, VALOR_SOLO_AGUA);

// --- Intervalo de Leitura ---
// O DHT22 precisa de pelo menos 2 segundos. A pausa solicitada é de 2s entre DHT e Solo.
// Então o ciclo total será um pouco maior que 2s + 2s.
// Vamos manter um intervalo principal para tentar ler o DHT.
const unsigned long INTERVALO_PRINCIPAL_LEITURA = 4000; // Tenta ler o conjunto a cada 4 segundos (2s DHT + 2s pausa)
unsigned long tempoAnteriorLeitura = 0;

// --- FUNÇÃO SETUP ---
void setup() {
  Serial.begin(9600);
  Serial.println(F("Monitor de Planta - Formato Detalhado"));
  dhtSensor.iniciar(); // Inicializa o sensor DHT
  Serial.println(F("------------------------------------"));
}

// --- FUNÇÃO LOOP ---
void loop() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnteriorLeitura >= INTERVALO_PRINCIPAL_LEITURA) {
    tempoAnteriorLeitura = tempoAtual; // Atualiza o tempo da última tentativa de leitura do conjunto

    Serial.println(F("\n>>> Leitura do Ambiente <<<"));

    // Tenta realizar uma nova leitura do DHT22
    if (dhtSensor.tentarNovaLeitura()) { 
      float umidade = dhtSensor.lerUmidadeAr();
      float tempC = dhtSensor.lerTemperaturaCelsius();
      float tempF = dhtSensor.lerTemperaturaFahrenheit();
      float hic = dhtSensor.calcularIndiceCalorCelsius();
      float hif = dhtSensor.calcularIndiceCalorFahrenheit();

      Serial.print(F("Umidade: "));
      Serial.print(umidade, 2); // Duas casas decimais
      Serial.println(F(" %"));

      Serial.print(F("Temperatura: "));
      Serial.print(tempC, 2);
      Serial.print(F(" °C / "));
      Serial.print(tempF, 2);
      Serial.println(F(" °F"));

      Serial.print(F("Índice de calor: "));
      Serial.print(hic, 2);
      Serial.print(F(" °C / "));
      Serial.print(hif, 2);
      Serial.println(F(" °F"));

    } else {
      Serial.println(F("Falha ao ler dados do sensor DHT22!"));
    }

    // Pausa de 2 segundos ANTES de ler o sensor de umidade do solo
    Serial.println(F("Aguardando 2 segundos..."));
    delay(2000);

    Serial.println(F("\n>>> Leitura do Solo <<<"));
    int umidadeSolo = soloSensor.readMoisturePercentage();
 

    Serial.print(F("Umidade do Solo: "));
    Serial.print(umidadeSolo);
    Serial.println(F(" %"));

    Serial.println(F("------------------------------------"));
  }
}