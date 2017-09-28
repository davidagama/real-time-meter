

/* importaÃ§Ã£o necessÃ¡ria*/
#include <Wire.h>
#include <SoftwareSerial.h>


//int portaAnalogica[8];
int pinoSensor1 = A0;
int pinoSensor2 = A1;
int sensorValue_aux = 0;
float valorSensor1 = 0;
float valorSensor2 = 0;
float valorCorrente1 = 0;
float valorCorrente2 = 0;
float voltsporUnidade = 0.004887586; // 5/1023
float sensibilidade = 0.100;
float kwh = 0;
float tarifa = 0.56715;
int constanteKwh = 1000;
int constanteHora = 24;
float aux = 0;
float potencia = 0;
float total = 0;
float custo = 0;

//tensÃ£o da rede AC 110 Volts e na verdade (127 volts)
int tensao = 127;
//int tensao = 0;



void setup () {

  //Inicia a Serial
  Serial.begin(9600);
  pinMode(pinoSensor1, INPUT);
  pinMode(pinoSensor2, INPUT);
  //Serial.println("Qual o valor da tensão: ");
  //tensao = Serial.read();


}

void loop() {

if(tensao != 0){
  for (int i = 10000; i > 0; i--) {
    sensorValue_aux = (analogRead (pinoSensor1) -  510);  // le o sensor
    valorSensor1 += pow(sensorValue_aux, 2); // soma os quadrados
  }
  valorSensor1 = (sqrt (valorSensor1 / 10000)) + voltsporUnidade;
  valorCorrente1 = (valorSensor1 / sensibilidade);

  //tratamento para possivel ruido no ACS712
  if (valorCorrente1 <= 0.095) {
    valorCorrente1 = 0;
  }

  for (int i = 10000; i > 0; i--) {
    sensorValue_aux = (analogRead (pinoSensor2) -  510);  // le o sensor
    valorSensor2 += pow(sensorValue_aux, 2); // soma os quadrados
  }
  valorSensor2 = (sqrt (valorSensor2 / 10000)) + voltsporUnidade;
  valorCorrente2 = (valorSensor2 / sensibilidade);

  //tratamento para possivel ruido no ACS712
  if (valorCorrente2 <= 0.095) {
    valorCorrente2 = 0;
  }

  valorSensor1 = 0;
  valorSensor2 = 0;



  //mostra o valor da corrente
  Serial.println("----------------------");
  Serial.print("Sensor na porta: ");
  Serial.println(pinoSensor1);
  Serial.print("Corrente: ");
  Serial.print(valorCorrente1, 2);
  Serial.println(" A.");

  //escreve e calcula a potencia
  Serial.print("Potencia (consumo): ");
  Serial.print(valorCorrente1 * tensao / constanteKwh);
  Serial.println(" Watts.");


  aux += (valorCorrente1 * tensao / constanteKwh) / constanteHora;
  kwh = aux;


  Serial.print("Total: ");
  Serial.print((kwh / constanteKwh), 4);
  Serial.println(" KWh");
  Serial.print("Custo: ");
  Serial.print((kwh * (tarifa) / constanteKwh), 3);
  Serial.println(" TemeR$");
  //Serial.println("----------------------");
  
  potencia = valorCorrente2 * tensao / constanteKwh;
  total = kwh / constanteKwh;
  custo = (kwh * tarifa) / constanteKwh;
  aux = 0;
  aux += (valorCorrente2 * tensao / constanteKwh) / constanteHora;
  kwh = aux;



  //mostra o valor da corrente
  Serial.println("----------------------");
  Serial.print("Sensor na porta: ");
  Serial.println(pinoSensor2);
  Serial.print("Corrente: ");
  Serial.print(valorCorrente2, 2);
  Serial.println(" A.");

  //escreve e calcula a potencia
  Serial.print("Potencia (consumo): ");
  //Serial.print(valorCorrente2 * tensao / constanteKwh);
  Serial.print(potencia);
  Serial.println(" Watts.");

  Serial.print("Total: ");
  // Serial.print((kwh / constanteKwh), 4);
  Serial.print(total);
  Serial.println(" KWh");
  Serial.print("Custo: ");
  //Serial.print((kwh * (tarifa) / constanteKwh), 3);
  Serial.print(custo);
  Serial.println(" TemeR$");
  Serial.println("----------------------");

  delay (1000);
}

}


