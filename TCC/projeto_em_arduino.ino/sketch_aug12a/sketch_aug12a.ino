//código arduino

#include <ESP8266WiFi.h>
// configuração do AP
String AP_NameString = "XPToN.com";
const char WiFiAPPSK[] = "Guarda";
// Pino de ligação do Led
const int LED_PIN = 4;

int val = -1;

//Configuração servidor
WiFiServer server(80);
void setup() {

  initHardware();
  setupWiFi();
  server.begin();
}

int value = 0;

void loop() {
  // Verifica se há clientes ligados…
  WiFiClient clientS = server.available();
  if (!clientS) {
    return;
  }

  // Read the first line of the request
  String req = clientS.readStringUntil('\r');
  Serial.println(req);
  clientS.flush();
  val = -1;
  if (req.indexOf(" / led / 0") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf(" / led / 1") != -1)
    val = 1; // Will write LED high
  if (val >= 0)
    digitalWrite(LED_PIN, val);

  clientS.flush();

  // Resposta ao cliente
  String s = "HTTP / 1.1 200 OK\r\n";
s += "Content - Type: text / html\r\n\r\n";
  s += " < !DOCTYPE HTML > \r\n<html>\r\n";

  if (val >= 0)
  {
    s += "O LED está ";
    s += (val) ? "ligado" : "desligado";
  }

  s += "<h1>PPLWARE ESP8266 by XPToN.com < / h1 > <p>Led < a href = \"led / 1\" > <button>Ligar < / button > < / a > &nbsp; < a href = \"led / 0\" > <button>Desligar < / button > < / a > < / p > ";
  s += " < / html > \n";

  // envia codigo HTML
  clientS.print(s);
  delay(1);
  Serial.println("Client disonnected");

}
void setupWiFi()
{

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
  String(mac[WL_MAC_ADDR_LENGTH -1], HEX);
  macID.toUpperCase();

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(115200);
  //pinMode(LED_PIN, OUTPUT);
  //digitalWrite(LED_PIN, LOW);

  //configurar a placa para modo AP
  WiFi.mode(WIFI_AP);
}
