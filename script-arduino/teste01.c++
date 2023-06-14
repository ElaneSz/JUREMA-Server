#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SimpleDHT.h>

// Configurações de rede Wi-Fi
const char* ssid = "teste";
const char* password = "testeia212022";

// Configurações do servidor
const char* serverURL = "http://192.168.1.100:"; // Talvez não precise do ":"
const int serverPort = 3000;
const String endpoint = "/medicao";

// Pino do sensor DHT11
const int dht11Pin = D2;

SimpleDHT11 dht11;

void setup() {
  Serial.begin(115200);

  // Conecta-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi!");

  delay(1000);
}

void loop() {
  // Realiza a leitura do sensor DHT11
  byte umidade = 0;
  byte temperatura = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(dht11Pin, &umidade, &temperatura, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Falha na leitura do DHT11, err=");
    Serial.println(err);
    delay(2000);
    return;
  }

  // Valor fixo do ninho
  int ninho = 1;

  // Cria o JSON com os dados
  String jsonData = "{\"umidade\":" + String(umidade) + ",\"temperatura\":" + String(temperatura) + ",\"ninho\":" + String(ninho) + "}";

  // Realiza o POST dos dados para o servidor
  WiFiClient client;
  if (client.connect(serverURL, serverPort)) {
    client.print("POST " + serverUrl + ":" + serverPort + endpoint + " HTTP/1.1\r\n");
    client.print("Host: " + String(serverURL) + "\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(jsonData.length()) + "\r\n"); // Coloca o 'jsonData' no .body
    client.print("\r\n");
    client.print(jsonData);
    client.print("\r\n");

    Serial.println("Dados enviados para o servidor!");

    client.stop();
  } else {
    Serial.println("Falha na conexão com o servidor!");
  }

  delay(5000); // Espere 5 segundos antes de fazer a próxima leitura
}
