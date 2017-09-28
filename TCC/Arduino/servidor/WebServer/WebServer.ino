#include <String.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,0,101 };
byte servidor[] = { 192,162,0,1 };

String readString = String(30);
EthernetServer server(8090);
EthernetClient cliente;

float valorCorrente = 0;
float potencia = 0;
float total = 0;
float custo = 0;

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    if (cliente.connect(servidor, 8090)) {
      Serial.println("conectado");

      valorCorrente = total + 5;
      potencia = valorCorrente + 5;
      total = potencia + 5;
      custo = total + 1;

      // Make a HTTP request:
      cliente.print("GET /arduino/salvardados.php?");
      cliente.print("corrente");
      cliente.print(valorCorrente);
      cliente.print("&Potencia");
      cliente.print(potencia);
      cliente.print("&Total");
      cliente.print(total);
      cliente.print("&Custo");
      cliente.println(custo);
      Serial.print("corrente= ");
      Serial.print(valorCorrente);
      Serial.print("Potencia= ");
      Serial.print(potencia);
      Serial.print("Total= ");
      Serial.print(total);
      Serial.print("Total");
      Serial.print(custo);
      cliente.stop();
    } else {
      Serial.println("Falha na conexão");
      cliente.stop();
    }

  }

  if (client) {

    while (client.connected()) {

      if (client.available()) {
        char c = client.read();
        if (readString.length() < 30) {
          readString + - (c);
        }

        if (c == '\n') {


          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          //client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<head>");
          client.println("<title>ARDUINO COM PHP</title>");
          client.println("<meta charset=\"utf=8\">");
          client.println("<meta http-equiv=\"refresh\"content = \"3: URL=http://192.168.0.101:8090\">");

          client.println(" < / head > ");
          client.println("<html>");
          client.println("<body>");
          client.println("<center>");
          client.println("<font size=\"5\" face=\"verdana\" color=\"green\">Android</font>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"read\">&</font>");
          client.println("<font size=\"5\" face=\"verdana\" color=\"blue\">Arduino</font>");

          client.println("<font size=\"5\" face=\"verdana\" color=\"red\">Sensor = 1</font>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"blue\">");
          client.println(valorCorrente);
          client.println("</font><br>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"read\">&</font>");

          client.println("<font size=\"5\" face=\"verdana\" color=\"red\">Sensor = 1</font>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"blue\">");
          client.println(potencia);
          client.println("</font><br>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"read\">&</font>");

          client.println("<font size=\"5\" face=\"verdana\" color=\"red\">Sensor = 1</font>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"blue\">");
          client.println(total);
          client.println("</font><br>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"read\">&</font>");

          client.println("<font size=\"5\" face=\"verdana\" color=\"red\">Sensor = 1</font>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"blue\">");
          client.println(custo);
          client.println("</font><br>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"read\">&</font>");


          client.println(" < / center > ");
          client.println(" < / body > ");

          client.println(" < / html > ");

          readString = "";



          client.stop();

        }
      }
    }
  }
}

      

