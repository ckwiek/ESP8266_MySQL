/* ESP8266 e Banco de Dados - Requisições HTTP
 * 2016 por José Cintra
 * www.josecintra.com/blog
 * Adaptado para video por Carlos kwiek
 * Link video https://www.youtube.com/watch?v=o_CUGWhC-3E&feature=youtu.be
 */ 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SimpleDHT.h>

// WiFi - Coloque aqui suas configurações de WI-FI
const char ssid[] = "rede";
const char psw[] = "senha";

// Site remoto - Coloque aqui os dados do site que vai receber a requisição GET
const char http_site[] = "http://localhost/phpmyadmin/";
const int http_port = 80;

// Variáveis globais
WiFiClient client;
IPAddress server(192,168,X,XX); //Endereço IP do servidor - http_site ver com ipconfig/all no IPV4 (preferencial)
int pinDHT11 = D1;
SimpleDHT11 dht11;

void setup() {
  delay(30000); //Aguarda 30 segundos 
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
  byte temp = 0;
  byte humid = 0;
  if (dht11.read(pinDHT11, &temp, &humid, NULL)) {
    Serial.print("Falha na leitura do sensor.");
    return;
  }
  
  Serial.println("Gravando dados no BD: ");
  Serial.print((int)temp); Serial.print(" *C, "); 
  Serial.print((int)humid); Serial.println(" %");

  // Envio dos dados do sensor para o servidor via GET
  if ( !getPage((int)temp,(int)humid) ) {
    Serial.println("GET request failed");
  }
}

// Executa o HTTP GET request no site remoto
bool getPage(int temp, int humid) {
  if ( !client.connect(server, http_port) ) {
    Serial.println("Falha na conexao com o site ");
    return false;
  }
  String param = "/?temp=" + String(temp) + "&humid=" + String(humid); //Parâmetros com as leituras
  Serial.println(param);
  client.println("GET /weather/insert_weather.php" + param + " HTTP/1.1");
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
