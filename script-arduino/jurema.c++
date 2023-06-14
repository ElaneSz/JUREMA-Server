#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SimpleDHT.h>

// WiFi - Coloque aqui suas configurações de WI-FI
const char ssid[] = "teste";
const char psw[] = "testeia212022";

// Site remoto - Coloque aqui os dados do site que vai receber a requisição GET
const char* serverURL = "http://localhost";
const int serverPort = 80;
const String endpoint = "/medicao";

// Variáveis globais
WiFiClient client;
IPAddress server(191, 168, 1, 100); //Endereço IP do servidor - http_site
int pinDHT11 = D2;
SimpleDHT11 dht11;

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("NodeMCU - Gravando dados no BD via GET");
  Serial.println("Aguardando conexão");
  
  // Tenta conexão com Wi-fi
  WiFi.begin(ssid, psw);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\nWI-FI conectado com sucesso: ");
  Serial.println(ssid);

}

void loop() {
  
  //Leitura do sensor DHT11
  delay(3000); //delay entre as leituras
  byte umidade = 0;
  byte temperatura = 0;
  int ninho = 1; //Número do ninho
  if (dht11.read(pinDHT11, &temperatura, &umidade, NULL)) {
    Serial.print("Falha na leitura do sensor.");
    return;
  }
   
  Serial.println("Gravando dados no BD: ");
  Serial.print((int)temperatura); Serial.print(" *C, "); 
  Serial.print((int)umidade); Serial.println(" %");
  Serial.print((int)ninho); 
  // Envio dos dados do sensor para o servidor via GET
  if (!getPage((int)umidade,(int)temperatura)) {
    Serial.println("GET request failed");
  }
}


  // byte umidade = 0;
  // byte temperatura = 0;
  // byte ninho = 1; 

// Cria o JSON com os dados
  

// Executa o HTTP GET request no site remoto
bool getPage(int umidade, int temperatura) {
  String jsonData = "{\"umidade\":" + String(umidade) + ",\"temperatura\":" + String(temperatura) + ",\"ninho\":" + String(ninho) + "}";

  if (!client.connect(serverURL, 80)) {
    Serial.println("Falha na conexao com o site ");
    return false;
  }
    client.print("POST http://192.168.1.100:80/medicao HTTP/1.1\r\n");
    client.print("Host: " + String(serverURL) + "\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(jsonData.length()) + "\r\n"); // Coloca o 'jsonData' no .body
    client.print("\r\n");
    client.print(jsonData);
    client.print("\r\n");

    // Informações de retorno do servidor para debug
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  return true;
}