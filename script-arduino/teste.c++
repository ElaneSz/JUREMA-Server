#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SimpleDHT.h>

// Configurações de rede Wi-Fi
const char* ssid = "Moto G (5) 3100";
const char* password = "arduino03";

// Configurações do servidor
const char* serverURL = "192.168.43.89";
const int serverPort = 3000;
const String endpoint = "/medicao";

// Pino do sensor DHT11
const int dht11Pin = D2;

// Pino da LED vermelha
const int ledPin = D4;

SimpleDHT11 dht11;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Desliga a LED no início

  // Conecta-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi!");

  delay(1000);
}

void blinkLED(int times, int delayTime) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    delay(delayTime);
  }
}

void loop() {
  // Realiza a leitura do sensor DHT11
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(dht11Pin, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Falha na leitura do DHT11, err=");
    Serial.println(err);

    // Caso ocorra falha na leitura, mantém a LED ligada
    digitalWrite(ledPin, HIGH);
    delay(2000); // Espera 2 segundos antes de tentar novamente

    return;
  }

  // Valor fixo do ninho
  int ninho = 1;

  // Cria o JSON com os dados
  String jsonData = "{\"umidade\":" + String(humidity) + ",\"temperatura\":" + String(temperature) + ",\"ninho\":" + String(ninho) + "}";

  // Inicializa a conexão Wi-Fi
  WiFiClient client;
  if (client.connect(IPAddress(192, 168, 43, 89), serverPort)) {
    // Realiza a requisição HTTP POST
    HTTPClient http;
    http.begin(client, serverURL, serverPort, endpoint);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(jsonData);
    if (httpCode > 0) {
      Serial.println("Dados enviados para o servidor!");

      // Faz a LED piscar 3 vezes, com intervalo de 500ms
      blinkLED(3, 500);
    } else {
      Serial.println("Falha na requisição HTTP!");
    }
    http.end();
  } else {
    Serial.println("Falha na conexão com o servidor!");

    // Caso ocorra falha na conexão, mantém a LED ligada
    digitalWrite(ledPin, HIGH);
  }

  delay(5000); // Espere 5 segundos antes de fazer a próxima leitura
}


