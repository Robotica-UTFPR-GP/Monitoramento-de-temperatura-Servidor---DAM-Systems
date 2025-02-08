#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

// Configuração do Ethernet Shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);  // Troque conforme necessário

EthernetServer server(80);

// Configuração do DHT
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  dht.begin();
  Serial.print("Servidor iniciado em: ");
  Serial.println(ip);
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado!");

    // Leitura dos dados do sensor
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Se a leitura falhar, exibe um erro
    if (isnan(h) || isnan(t)) {
      Serial.println("Falha na leitura do DHT!");
      return;
    }

    // Cabeçalhos HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    // HTML com JavaScript para atualizar automaticamente
    client.println("<!DOCTYPE html>");
    client.println("<html lang='pt'>");
    client.println("<head>");
    client.println("<meta charset='UTF-8'>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<title>Monitoramento</title>");
    client.println("<script>");
    client.println("setInterval(() => { location.reload(); }, 2000);");  // Atualiza a cada 2s
    client.println("</script>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h2>Monitoramento de Temperatura e Umidade</h2>");
    client.println("<p>Temperatura: " + String(t) + " °C</p>");
    client.println("<p>Umidade: " + String(h) + " %</p>");
    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
  }
}
