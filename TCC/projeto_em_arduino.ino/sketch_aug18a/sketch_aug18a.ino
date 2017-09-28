// Servidor web stand-alone com Arduino e ESP8266 - Augusto Campos 2015

#define debug
#define rede "pinoquio"
#define senha "mentira"

const byte maxPagina=80;
const byte linhasPagina=7;
const char htmlCorpo[linhasPagina] [maxPagina] PROGMEM = {
 { "<head><title>BR-Arduino ESP8266</title><style>#q{float:left;height:240px;"         },
 { "width:240px;margin:10px;border-radius:12px;background:#336699;}#l{font-size:"      },
 { "64px;color:#ffcc66;margin-top:82px;width:100%;height:32px;text-align:center;}"     },
 { "body{width: 260px; margin-left: auto; margin-right: auto; border:"                 },
 { "1px solid #d0d0d0;font-family:sans-serif;font-size:32px;font-weight:bold;"         },
 { "border-radius:25px;text-align:center;}a{text-decoration:none}</style>"             },
 { "</head><body><b>BR-Arduino.org</b><p>"                                             }
};

int montaPagina(char *url, char *pagina) {
  byte tipo=255;
  if (0==strcmp(url,"/1")) tipo=0;
  else if (0==strcmp(url,"/3")) tipo=1;
  else if (0==strcmp(url,"/d")) tipo=2;
  else if (0==strcmp(url,"/ac")) tipo=3;
  else if (0==strcmp(url,"/ap")) tipo=4;
  pagina[0]='∖0';
  for (byte i=0; i<linhasPagina; i++) {  
    char S[maxPagina];
    memcpy_P(&S,&htmlCorpo[i],sizeof S);
    strcat(pagina,S);
  }
  char umSensor[3];
  const char* predivs="∖n∖r<div id=q><div id=l>";
  const char* posdivs="</div></div>";
  const char* sepmenu="</a><br><a href=/";
  switch (tipo) {
  case 0:    
    itoa(analogRead(A3),umSensor,10);
    strcat(pagina,predivs);
    strcat(pagina,umSensor);
    strcat(pagina,posdivs);
    break;
  case 1:    
    itoa(analogRead(A3),umSensor,10);
    strcat(pagina,predivs);
    strcat(pagina,umSensor);
    strcat(pagina,posdivs);
    itoa(analogRead(A4),umSensor,10);
    strcat(pagina,predivs);
    strcat(pagina,umSensor);
    strcat(pagina,posdivs);
    itoa(analogRead(A5),umSensor,10);
    strcat(pagina,predivs);
    strcat(pagina,umSensor);
    strcat(pagina,posdivs);
    break;
  case 2:    
    itoa(analogRead(A3),umSensor,10);
    strcat(pagina,predivs);
    strcat(pagina,umSensor);
    strcat(pagina,posdivs);
    itoa(analogRead(A4),umSensor,10);
    strcat(pagina,predivs);
    strcat(pagina,umSensor);
    strcat(pagina,posdivs);
    strcat(pagina,predivs);
    if (digitalRead(12)==HIGH) strcat(pagina,"HIGH");
    else strcat(pagina,"LOW");
    strcat(pagina,posdivs);
    break;
  case 3:
    digitalWrite(13,HIGH);
    strcat(pagina,predivs);
    strcat(pagina,"OK ♦");
    strcat(pagina,posdivs);
    break;
  case 4:    
    digitalWrite(13,LOW);
    strcat(pagina,predivs);
    strcat(pagina,"OK ◊");
    strcat(pagina,posdivs);
    break;
  default:
    strcat(pagina,"<a href=/ac>acende");
    strcat(pagina,sepmenu);
    strcat(pagina,"ap>apaga");
    strcat(pagina,sepmenu);
    strcat(pagina,"1>1 sensor");
    strcat(pagina,sepmenu);
    strcat(pagina,"3>3");
    strcat(pagina,sepmenu);
    strcat(pagina,"d>c/ digital</a>");
  } 
  return strlen(pagina);  
}  

// a partir daqui o codigo eh generico
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv \

const byte CH_PD=5;
const byte RST=6;

#include <SoftwareSerial.h>
SoftwareSerial monitorSerial(11, 10); // RX, TX

boolean aguardaResposta(const char *sucesso, unsigned long limite=7000, boolean mostrar=false) {
  char resp[90];
  unsigned long chegada=millis();
  unsigned long tempo;
  boolean continuar=true; 
  boolean timeout=false;
  int contaChars=0;
  while (continuar) { 
    if (Serial.available()) {
      resp[contaChars] = Serial.read();
      contaChars++;
      if (contaChars>88) contaChars=0;  // aqui deveria haver uma condicao de erro
#ifndef debug
      if (mostrar) {
        monitorSerial.print(resp[contaChars-1]);
      }
#else
      monitorSerial.print(resp[contaChars-1]);
#endif      
      if (resp[contaChars-1]==10) {  // LF, fim da linha recebida
        if (contaChars>1) {
          resp[contaChars-2]='∖0';
          if (0==strcmp(resp,"OK")) continuar=false;
          else if (0==strcmp(resp,"SEND OK")) continuar=false;
          else if (0==strcmp(resp,"ready")) continuar=false;
          else if (0==strcmp(resp,"error")) continuar=false;
          else if (0==strcmp(resp,"ERROR")) continuar=false;
          else if (0==strcmp(resp,"Unlink")) continuar=false;
          contaChars=0;
        }  
      }  
    }  
    tempo=millis()-chegada;
    if (tempo > limite) {
      timeout=true;
      continuar=false;
    }  
  }
  boolean retorno=false;
  if (!timeout & (0==strcmp(resp,sucesso))) retorno=true;  
  return retorno;
} 

void descreve(const char *msg) {
  monitorSerial.print("** ");
  monitorSerial.println(msg);
}

void erro(int codigo, const char *msg) {
  monitorSerial.print("!! ERRO ");
  monitorSerial.println(msg);
  if (codigo<100) {
    descreve("Pressione RESET.");
    while(true) {  /* loop vazio sem fio para travar execucao */ } 
  }  
}

boolean resetESP() {
  descreve("Reset fisico ESP");
  digitalWrite(RST,LOW);
  delay(100);
  digitalWrite(RST,HIGH);
  delay(3000);
  descreve("Reset logico ESP");
  Serial.println("AT+RST");  
  aguardaResposta("OK",200); // aqui soh confirma a recepcao do comando
  return aguardaResposta("ready",2000);
}  

boolean conectaWiFi() {
  descreve("Conec. wifi");
  char comando[100]="AT+CWJAP=\"";
  strcat(comando,rede);
  strcat(comando,"\",\"");
  strcat(comando,senha);
  strcat(comando,"\"");
  Serial.println(comando);  
  return aguardaResposta("OK",10000);
}  

boolean obtemIP() {
  descreve("Obtendo IP - use o segundo");
  Serial.println("AT+CIFSR");  
  return aguardaResposta("OK",5000,true);
}  

boolean ativaServidor() {
  descreve("Ativ. MUX");
  Serial.println("AT+CIPMUX=1");  
  if (aguardaResposta("OK",500)) {
    descreve("Ativ. porta 80");
    Serial.println("AT+CIPSERVER=1,80");  
    return aguardaResposta("OK",500);
  }  
  else return false;
}  

boolean recebeComandoWeb(char *idconexao, char *url) {
  const char *sucesso="OK";
  unsigned long limite=7000;
  unsigned long chegada=millis(); 
  char resp[514]="";
  unsigned long tempo;
  boolean continuar=true; 
  boolean timeout=false;
  int contaChars=0;
  while (continuar) { 
    if (Serial.available()) {
      unsigned long chegada=millis();   // recomeca a contar quando ha recepcao
      resp[contaChars] = Serial.read();
      contaChars++;
      if (contaChars>512) contaChars=0;  // aqui deveria haver uma condicao de erro
#ifdef debug
      monitorSerial.print(resp[contaChars-1]);
#endif
      if (resp[contaChars-1]==10) {  // LF, fim da linha recebida
        if (contaChars>1) {
          resp[contaChars-2]='∖0';
          if (0==strcmp(resp,"OK")) continuar=false;
          else if (0==strcmp(resp,"error")) continuar=false;
          else if (0==strcmp(resp,"ERROR")) continuar=false;
          else if (0==strcmp(resp,"Unlink")) continuar=false;
          else if (0==strcmp(resp,"Link")) continuar=false;
          else {
            if (strstr(resp,"+IPD,")) {
              descreve("Rec. requisicao web"); 
              // o ID da conexao fica entre virgulas
              char *conexaoStr=strstr(resp,",")+1;
              char *pos=strstr(conexaoStr,",");
              if (pos!=0) {
                strncpy(idconexao,conexaoStr,pos-conexaoStr);
                idconexao[pos-conexaoStr]='∖0';
                // a URL solicitada fica entre espacos
                char *urlStr=strstr(resp," ")+1;
                char *pos=strstr(urlStr," ");
                if (pos!=0) {
                  strncpy(url,urlStr,pos-urlStr);
                  url[pos-urlStr]='∖0';
                }  
              }  
            }
          }  
          contaChars=0;
        }  
      }  
    }  
    tempo=millis()-chegada; // tempo sem recepcao
    if (tempo > limite) {
      timeout=true;
      continuar=false;
    }  
  }
  boolean retorno=false;
  if (!timeout & (0==strcmp(resp,sucesso))) retorno=true; 
  return retorno;
}

void ESPsendStr(const char *conexao, const char *ESPlinha) {
  int comprimento=strlen(ESPlinha);
  char comando[30]="AT+CIPSEND=";
  strcat(comando,conexao);
  strcat(comando,",");
  Serial.print(comando);
  Serial.println(comprimento,DEC);
  Serial.find("> ");  
  Serial.print(ESPlinha);
  aguardaResposta("SEND OK");
}  

boolean servePagina(char *idconexao, char *url) {  
#ifdef debug
  monitorSerial.print(F("Servindo a pagina da URL "));
  monitorSerial.print(url);
  monitorSerial.print(F(" na conexao "));
  monitorSerial.println(idconexao);
#endif      
  char paginaMontada[600]="";
  int tamanhoHTML=montaPagina(url,paginaMontada);
  ESPsendStr(idconexao,"HTTP/1.1 200 OK∖r∖nContent-Type: text/html∖r∖n");
  ESPsendStr(idconexao,"Connection: close∖r∖nContent-Length: ");
  char tamanhoSend[4];
  itoa(tamanhoHTML,tamanhoSend,10);
  ESPsendStr(idconexao,tamanhoSend);
  ESPsendStr(idconexao,"∖r∖n∖r∖n");
  byte fatias=tamanhoHTML/78;
  for (byte f=0; f<=fatias; f++) {
    char buf[80];
    char *umaFatia=&paginaMontada[f*78];
    strncpy(buf,umaFatia,78);
    buf[78]='∖0';
    ESPsendStr(idconexao,buf);
  }  
  aguardaResposta("SEND OK");
}  

void setup() {
  // bloco 1 - inicializacao especifica da aplicacao
  pinMode(13,OUTPUT);
  pinMode(12,INPUT_PULLUP);
  // bloco 2 - inicializacao fisica generica para o ESP
  Serial.begin(9600);
  Serial.setTimeout(5000);
  monitorSerial.begin(38400);
  pinMode(RST,OUTPUT);
  pinMode(CH_PD,OUTPUT);
  digitalWrite(CH_PD,HIGH);
  // bloco 3 - inicializacao logica do ESP e operacao do servidor
  if (resetESP()) {
    if (conectaWiFi()) {
      if (obtemIP()) {
        if (ativaServidor()) {
          descreve("Aguardando conexoes");
          while (true) {   // loop sem fim
            char url[64]="";
            char idconexao[3]="";
            if (recebeComandoWeb(idconexao,url)) {
              if ((url[0]!='∖0') & (idconexao[0]!='∖0')) {
                servePagina(idconexao,url);
              }  
            }  
          }
        } 
        else erro(5,"servidor");
      } 
      else erro(4,"IP");
    } 
    else erro(3,"WiFi");
  } 
  else erro(2,"reset");  
}

void loop() {
  // intencionalmente vazio
}
