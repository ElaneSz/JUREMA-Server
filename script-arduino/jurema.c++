#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SimpleDHT.h>
#include <ArduinoJson.h>

// WiFi - Coloque aqui suas configurações de WI-FI
const char ssid[] = "teste";
const char psw[] = "testeia212022";

// Site remoto - Coloque aqui os dados do site que vai receber a requisição GET
const char http_site[] = "http://127.0.0.1:3000/medicao";
const int http_port = 3000;

// Variáveis globais
WiFiClient client;
IPAddress server(192, 168, 1, 100); //Endereço IP do servidor - http_site
int pinDHT11 = D2;
SimpleDHT11 dht11;

void setup() {
  delay(3000); //Aguarda 30 segundos 
  Serial.begin(9600);
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

byte temperatura = 0;
byte umidade = 0;
byte ninho = 1;  //Número do ninho
void loop() {
  
  //Leitura do sensor DHT11
  delay(3000); //delay entre as leituras
  if (dht11.read(pinDHT11, &temperatura, &umidade, NULL)) {
    Serial.print("Falha na leitura do sensor.");
    return;
  }
  
  float f_temperatura = temperatura;
  float f_umidade = umidade;
  int f_ninho = ninho;

  Serial.println("Gravando dados no BD: ");
  Serial.print((int)temperatura); Serial.print(" °C, "); 
  Serial.print((int)umidade); Serial.println(" %");
  char json[1024];  //Parâmetros com as leituras
  snprintf(json, sizeof(json), "{\"umidade\": %.2f,\"temperatura\": %.2f,\"ninho\": %i}", f_umidade, f_temperatura, f_ninho);
  Serial.println(json);
  Serial.print((int)ninho); 
  // Envio dos dados do sensor para o servidor via GET
  if ( !getPage((int)temperatura,(int)umidade,(int)ninho) ) {
    Serial.println("GET request failed");
  }
}

// Executa o HTTP GET request no site remoto
bool getPage(float temperatura, float umidade, int ninho) {
  if ( !client.connect(server, http_port) ) {
    Serial.println("Falha na conexao com o site ");
    return false;
  }
  char json[1024];  //Parâmetros com as leituras
  snprintf(json, sizeof(json), "{\"umidade\": %.2f,\"temperatura\": %.2f,\"ninho\": %i}", umidade, temperatura, ninho);
  // Serial.println(json);
  client.println("GET http://127.0.0.1:3000/medicao"+ json +"HTTP/1.1");
  client.println("Host: ");
  client.println(http_site);      
  client.println("Connection: close");   
  client.println();        
  client.println();

    // Informações de retorno do servidor para debug
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  return true;
}