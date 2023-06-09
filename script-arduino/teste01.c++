#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h> // Outra biblioteca

// Configurações de rede
const char* ssid = "teste"; // Nome da sua rede Wi-Fi
const char* password = "testeia212022"; // Senha da sua rede Wi-Fi
const char* serverIP = "192.168.1.100"; // Endereço IP do servidor
int serverPort = 3000; // Porta do servidor

// Configurações do sensor DHT11
#define DHTPIN D2 // Pino ao qual o sensor DHT11 está conectado
#define DHTTYPE DHT11 // Tipo de sensor DHT

// Objeto DHT para ler os valores do sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Conecte-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado à rede Wi-Fi");

  // Inicialize o sensor DHT
  dht.begin();
}

void loop() {
  // Leia os valores de umidade e temperatura do sensor DHT11
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  bool ninho = 1; // Exemplo de valor do ninho

  // Verifique se a leitura do sensor foi bem-sucedida
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler os valores do sensor DHT11");
    return;
  }

  // Crie o JSON com os dados
  String jsonData = "{\"umidade\":" + String(umidade) + ",\"temperatura\":" + String(temperatura) + ",\"ninho\":" + String(ninho) + "}";

  // Faça a solicitação POST
  HTTPClient http;
  http.begin(serverIP, serverPort, "/medicao"); // Substitua pelo seu path
  http.addHeader("Content-Type", "application/json");
  int statusCode = http.POST(jsonData);

  // Obtenha a resposta do servidor
  String response = http.getString();

  // Imprima os resultados
  Serial.print("Código de status: ");
  Serial.println(statusCode);
  Serial.print("Resposta: ");
  Serial.println(response);

  delay(5000); // Espere 5 segundos antes de fazer a próxima solicitação
}
