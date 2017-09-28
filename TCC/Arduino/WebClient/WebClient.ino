#include <checksum.h>
#include <enc28j60.h>
#include <enc28typedef.h>
#include <EtherEncLib.h>
#include <TcpStack.h>


#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192,168,0,101};
byte servidor[] = {127,162,0,1};

EthernetClient cliente;

float valorCorrente = 0;
float potencia = 0;
float total = 0;
float custo = 0;

void setup() {
  Serial.begin(9600);

  Ethernet.begin(mac, ip);


}

void loop() {
  char comando = Serial.read();
  if (comando == 'a') {
    if (cliente.connect(servidor, 8090)) {
      Serial.println("conectado");

      //sensor1 = sensor3 +5;
      //sensor2 = sensor1 +5;
      //sensor3 = sensor2 + 5;
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
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }

  }
}

