/*
Programa de Controle de iluminação
Criado por Marcelo Maurin Martins
COMLINK
*/

#define Arduino2TX  38 //Define o pino do Arduino a ser utilizado com o pino Trigger do sensor
#define Arduino2RX    37 //Define o pino do Arduino a ser utilizado com o pino Echo do sensor

#include <stdlib.h>
#include <EmonLib.h>                   // Include Emon Library
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROMex.h>
#include <EEPROMvar.h>
//#include "Arduino.h"


#if ARDUINO >= 100
#include <SoftwareSerial.h>
#else
#include <NewSoftSerial.h>
#endif
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
#if ARDUINO >= 100
SoftwareSerial mySerial(Arduino2TX, Arduino2RX);
#else
NewSoftSerial mySerial(Arduino2TX, Arduino2RX);
#endif

const int maxAllowedWrites = 10000;
const int memBase          = 120;
int addressFloat; //Endereco da Corrente acumulada
int addressDHCP; //Endereço do DHCP
//addressSSH
int addressSSH; //Endereco do flgSSH


int addressIP; //IP fixo do Equipamento

int TEMPOLEITURAS = 10; //TEMPO ENTRE LEITURAS EM SEGUNDOS
int DIFCARGA = 1; //Diferença entre Leituras de Corrente (watts)

char Produto[15] = "MBREntrada";
String Buffer; //Buffer de digitação da Porta Serial

char inByte;

//Vetores controlados na EPROM
//Versao(1) Revisao(1) Lote(3) Id(3), notacao Hexadecimal


IPAddress ip(10, 0, 0, 195);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress srvdns(10, 0, 0, 10);


int Portclient = 8080;

bool flgDHCP = true; //Controle de start de dhcp

bool flgSSH  = true;



EthernetClient clientssh; //Client SSH
// telnet defaults to port 23
EthernetServer serverSSH(23); //Servidor SSH

boolean gotAMessage = false; // whether or not you got a message from the client yet



unsigned long timeslace = 0;


bool flgMonitor = true;

byte PinCorr = 1;
byte Pinrele01 = 6;
byte Pinrele02 = 7;

byte PinRFA = 31;
byte PinRFB = 35;
byte PinRFC = 37;
byte PinRFD = 39;

EnergyMonitor emon1;                   // Create an instance
float wattsacum = 0; //Corrente acumulada lida
double Corrente = 0;
double lastCorrente = 0;





// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};



void EnviaArduino(String info)
{
  mySerial.println(info);
}

//Le Carga Total sem perder valor anterior
double GetCarga(void)
{
  float oldValue = EEPROM.readFloat(addressFloat);
  //wattsacum = EEPROM.readFloat(addressFloat);
  //Serial.println("Leu valor na EEPROM!");
  return oldValue;
}

//Le DHCP
bool GetDHCP(void)
{
  byte oldValue = EEPROM.read(addressDHCP);
  flgDHCP = (oldValue != 0 ? true : false);
  flgDHCP = true;
  return flgDHCP;
}

//Le flgSSH
bool GetSSH(void)
{
  byte oldValue = EEPROM.read(addressSSH);
  flgSSH = (oldValue != 0 ? true : false);
  return flgSSH;
}

//grava valor SSH
void SetSSH(bool valor)
{
  EEPROM.write(addressSSH, (valor == true ? 255 : 0));
  flgSSH = (GetSSH() != 0 ? true : false);
  //Serial.println("Gravou SSH!");
}

//Le Carga Total
void SetDHCP(bool valor)
{
  Serial.println(valor == true ? "Habilitou DHCP" : "Desativou DHCP");
  EEPROM.write(addressDHCP, valor);
  flgDHCP = (GetDHCP() != 0 ? true : false);
  flgDHCP = true; //forca
  Serial.println("Gravou DHCP!");
}

//Le Carga Total
void SetCarga(float valor)
{
  EEPROM.writeFloat(addressFloat, valor);
  wattsacum = GetCarga();
  Serial.println("Gravou EEPROM!");
}



//Testa se o valor da carga chegou no limite para gravar na EEPROM
void TestaCarga(double Valor)
{
  //Verifica se a diferenca da carga entre o atual e o gravado
  //é maior que o padrao para gravacao
  if ((Valor - GetCarga()) > DIFCARGA)
  {
    Serial.println("Gravou!");
    SetCarga(Valor);
  }
}

void Ver()
{

  Print("Produto:");
  Println(String(Produto));
}

void VerClient(EthernetClient client)
{
  PrintClient(client, "Produto");
  PrintlnClient(client, String(Produto));
}

void Start_EEPROM()
{
  Serial.println("Inicializando EEPROM...");
  // start reading from position memBase (address 0) of the EEPROM. Set maximumSize to EEPROMSizeUno
  // Writes before membase or beyond EEPROMSizeUno will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMemPool(memBase, EEPROMSizeMega);

  // Set maximum allowed writes to maxAllowedWrites.
  // More writes will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMaxAllowedWrites(maxAllowedWrites);
  //Cria o endereco do float
  addressFloat  = EEPROM.getAddress(sizeof(float));
  addressDHCP   = EEPROM.getAddress(sizeof(byte));
  addressSSH   = EEPROM.getAddress(sizeof(byte));


  //SetDHCP(false);
  //SetSSH(false);


  flgDHCP = GetDHCP();
  flgDHCP = true; //forca
  flgSSH = GetSSH(); //Pega SSH


  //obtem o valor da carga
  wattsacum = GetCarga();
}

//Inicia a Serial
void Start_Serial()
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  Buffer = ""; //Inicializa Buffer Serial
}

void Start_Reles()
{
 //Pinrele01 
  pinMode(Pinrele01, OUTPUT);
  pinMode(Pinrele02, OUTPUT);
  digitalWrite(Pinrele01,LOW);
  digitalWrite(Pinrele02,LOW);
 
}

void Start_RF()
{
 //PinRFA 
 pinMode(PinRFA,INPUT);
 pinMode(PinRFB,INPUT);
 pinMode(PinRFC,INPUT);
 pinMode(PinRFD,INPUT);
 
}

void Start_Ethernet()
{

  Serial.println("Starting Ethernet...");
  if (flgDHCP == true)
  {
    // start the Ethernet connection:
    Serial.println("Estabelecendo DHCP");
    if (Ethernet.begin(mac) == 0)
    {
      Serial.println("Falha no Client DHCP");
      // initialize the ethernet device not using DHCP:
      Ethernet.begin(mac, ip, srvdns, gateway, subnet);

    }
  }
  else
  {
    Ethernet.begin(mac, ip, gateway, subnet, dns);
  }

  IFConfig();
}

void Start_SSH()
{
  if (flgSSH)
  {
    Serial.println("Inicializando Servidor SSH");
    serverSSH.begin();
    Buffer = "";
  }
}

void Start_Corr()
{
  Serial.println("Iniciando Sensor de Corrente");
  emon1.current(PinCorr, 5.2);             // Current: input pin, calibration.
  timeslace = millis();
  //correnteacum1 = GetCurrent();
  //correnteacum1 = 0;
}

void setup() {
  Start_Serial();
  Serial.println(Produto);
  Serial.println("Loading firmware...");
  Start_EEPROM();
  Start_Corr();
  Start_Reles();
  //Inicia a Ethernet
  Start_Ethernet();
  Start_RF();

  Start_SSH(); //Iniciando Servidor SSH

  //Finaliza
  Wellcome();
}

//Status
void STATUS()
{
  Serial.println("Status dos servicos");
  IFConfig(); //Status de IP


  if (flgSSH )
  {
    Serial.println("Servico de SSH ativo");
  }
  else
  {
    Serial.println("Servico de SSH inativo");
  }


}

//Status
void STATUSClient(EthernetClient client)
{
  client.println("Status dos servicos");
  IFConfigClient(client); //Status de IP

  if (flgSSH )
  {
    client.println("Servico de SSH ativo");
  }
  else
  {
    client.println("Servico de SSH inativo");
  }


}

void Rele01()
{
 byte Rele01 =  digitalRead(Pinrele01);
 
 if ((Rele01 == HIGH))
 {
   digitalWrite(Pinrele01, LOW);
 }
 else
 {
    digitalWrite(Pinrele01, HIGH);
 }
}

void Rele01ON()
{ 
   digitalWrite(Pinrele01, HIGH);
}

void Rele01OFF()
{ 
   digitalWrite(Pinrele01, LOW);
}

void Rele02()
{
 byte Rele02 =  digitalRead(Pinrele02);
 
 if ((Rele02 == HIGH))
 {
   digitalWrite(Pinrele02, LOW);
 }
 else
 {
    digitalWrite(Pinrele02, HIGH);
 }
}


void Le_RF()
{
 //PinRFA 
 byte RFA = digitalRead(PinRFA);
 byte RFB = digitalRead(PinRFB);
 byte RFC = digitalRead(PinRFC);
 byte RFD = digitalRead(PinRFD);
 Serial.println(RFC);
 if (RFC != 0)
 {
   Buffer = "RELE01\n"; 
 }
 if (RFD != 0)
 {
   Buffer = "RELE02\n"; 
 }  
}

//Mostra o ip da maquina
void IFConfig()
{
  // print your local IP address:
  Serial.print("Ethernet Device, IP HOST: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Mac Address:");
  for (byte thisByte = 0; thisByte < 6; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(mac[thisByte], HEX);
    Serial.print(":");
  }
  Serial.println();

}

//Mostra o ip da maquina
void IFConfigClient(EthernetClient client)
{
  // print your local IP address:
  client.print("Ethernet Device, IP HOST: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    client.print(ip[thisByte], DEC);
    client.print(".");
  }
  client.println();
  client.print("Mac Address:");
  for (byte thisByte = 0; thisByte < 6; thisByte++) {
    // print the value of each byte of the IP address:
    client.print(mac[thisByte], HEX);
    client.print(":");
  }
  client.println();

}

void Prompt()
{
  Serial.println(" ");
  Serial.print("$>");
}

void PromptClient(EthernetClient client)
{
  PrintlnClient(client, " ");
  PrintClient(client, "$>");
}

void Wellcome()
{
  Serial.println(Produto);
  Prompt();
}

void WellcomeClient(EthernetClient client)
{
  PrintlnClient(client, Produto);
  PromptClient(client);
}


//Zera corrente
void ZEROCORR()
{
  SetCarga(0.0);
}

void Carrega_corr()
{
  Corrente = emon1.calcIrms(1480);  // Calculate Irms only
  //Compara para registrar historico de corrente
  if (lastCorrente != Corrente)
  {
    lastCorrente = Corrente;
    //Registra logCorrente
  }

  float tempo =  (millis() - timeslace) / 1000;
  if (tempo > TEMPOLEITURAS)
  {
    /*
    Serial.print("Tempo:");
    Serial.println(tempo);
    */
    float watts = Corrente * 127;
    float wattsh = (watts * tempo) / 3600;
    /*
    Serial.print("Watts TEMP:");
    Serial.println(wattsh);
    Serial.print("Watts ATUAL:");
    Serial.println(wattsacum);
    */


    wattsacum += wattsh;
    //float (correnteacum1 / 3600);
    timeslace = millis();
    //SetCurrent(wattsacum);
    TestaCarga(wattsacum); //Verifica Se Carga pode ser gravada
  }
}

//Imprime sem quebra
void Print(String info)
{
  Serial.print(info);
  //Serial1.print(info);
}

void PrintClient(EthernetClient client, String info)
{
  client.print(info);
  //Serial1.print(info);
}

//Imprime com quebra
void Println(String info)
{
  Serial.println(info);
  //Serial1.println(info);
}

void PrintlnClient(EthernetClient client, String info)
{
  client.println(info);
  //Serial1.print(info);
}


//Controle de pré processador de texto
void  Preprocessor(String Resultado, String Info)
{
  Resultado = Info;
}

void Le_corr()
{
  Carrega_corr();
  //This relationship is wrong,According to demand, own calculations relationship
  Print("Corrente Atual: ");
  Println(String(Corrente));
  Print("Consumo Watts: ");
  char Info[20];
  //sprintf(Info,"%d",correnteacum1);
  dtostrf(wattsacum, 6, 2, Info);//wattsacum String(Corrente)
  Println(Info);
  //Serial.println(correnteacum1);

}

void Le_corrClient(EthernetClient client)
{
  Carrega_corr();
  //This relationship is wrong,According to demand, own calculations relationship
  PrintClient(client, "Corrente: ");
  PrintlnClient(client, String(Corrente));
  PrintClient(client, "Consumo Watts: ");
  char Info[20];
  dtostrf(wattsacum, 6, 2, Info);
  //sprintf(Info,"%d",correnteacum1);
  PrintlnClient(client, Info);
}



void Help()
{
  Println("MAN - Manual de comandos");
  Println("SERIAL - Envia dados pela serial, sintaxe SERIAL:Info<enter>");
  Println("SETMONITOR - Seta debug pela console, sintaxe SETMONITOR=ON,OFF");
  Println("SETDHCP - Habilita DHCP, sintaxe SETDHCP=ON,OFF");
  Println("SETSSH - Habilita Servico SSH de administracao SETSSH=ON,OFF");
  Println("CORR - Informa corrente Atual");
  Println("RELE01 - Inverte o valor do RELE01");
  Println("RELE01ON - ATIVA o valor do RELE01");
  Println("RELE01OFF -DESATIVA o valor do RELE01");
  Println("RELE02 - Inverte o valor do RELE02");
  Println("ZEROCORR - Zera contador de corrente");
  Println("VER - Versao do firmware");
  Println("IFCONFIG - ip da maquina");
  Println("STATUS - STATUS DO EQUIPAMENTO");
}

void HelpClient(EthernetClient client)
{
  PrintlnClient(client, "MAN - Manual de comandos");
  PrintlnClient(client, "SERIAL - Envia dados pela serial, sintaxe SERIAL:Info<enter>");
  PrintlnClient(client, "SETMONITOR - Seta debug pela console, sintaxe SETMONITOR=ON,OFF");
  PrintlnClient(client, "SETDHCP - Habilita DHCP, sintaxe SETDHCP=ON,OFF");
  PrintlnClient(client, "SETSSH - Habilita Servico SSH de administracao SETSSH=ON,OFF");
  PrintlnClient(client, "CORR - Informa corrente Atual");
  PrintlnClient(client, "RELE01 - Inverte o valor do RELE01");
  PrintlnClient(client, "RELE01ON - ATIVA o valor do RELE01");
  PrintlnClient(client, "RELE01OFF -DESATIVA o valor do RELE01");
  PrintlnClient(client, "RELE02 - Inverte o valor do RELE02");
  PrintlnClient(client, "ZEROCORR - Zera contador de corrente");
  PrintlnClient(client, "VER - Versao do firmware");
  PrintlnClient(client, "IFCONFIG - ip da maquina");
  PrintlnClient(client, "STATUS - STATUS DO EQUIPAMENTO");
}


void ExecCMD(String pBuffer)
{
  //Serial.print("Analisou:");
  //Serial.println(pBuffer);
  bool flgRodou = false;
  //ZEROCORR
  if (pBuffer.indexOf("ZEROCORR") >= 0)
  {
    ZEROCORR();
    Serial.println("Zerou a corrente!");
    flgRodou = true;
  }

  //Serial.print(pBuffer.indexOf("MSG"));
  if (pBuffer.indexOf("SERIAL:") >= 0)
  {
    EnviaArduino(pBuffer.substring(5));
    flgRodou = true;
  }


  //STATUS DAS CONEXOES
  if (pBuffer.indexOf("STATUS") >= 0)
  {
    STATUS();
    flgRodou = true;
  }

  if (pBuffer.indexOf("SETMONITOR=") >= 0)
  {
    //lcd.clear();
    //lcd.setCursor(0, 1);
    //lcd.print(pBuffer.substring(5));
    int Angulo = pBuffer.substring(6).toInt();
    Println(String(pBuffer.substring(11)));
    Println(String(pBuffer.substring(6).toInt()));
    //GBEsq(Angulo);
    if (pBuffer.substring(11) == "ON")
    {
      SETMONITOR(true);
    }
    if (pBuffer.substring(11) == "OFF")
    {
      SETMONITOR(false);
    }

    flgRodou = true;
  }

  if (pBuffer.indexOf("SETDHCP=") >= 0)
  {
    Serial.print("Parametro:");
    Serial.println(pBuffer.substring(8));
    if (pBuffer.substring(8) == "ON")
    {
      SetDHCP(true);
    }
    if (pBuffer.substring(8) == "OFF")
    {
      SetDHCP(false);
    }
    flgRodou = true;
  }

  if (pBuffer.indexOf("SETSSH=") >= 0)
  {
    Serial.print("Parametro:");
    Serial.println(pBuffer.substring(7));
    if (pBuffer.substring(7) == "ON")
    {
      SetSSH(true);
      Serial.println("Ativou SSH");
    }
    if (pBuffer.substring(7) == "OFF")
    {
      SetSSH(false);
      Serial.println("Destivou SSH");
    }
    flgRodou = true;
  }

  if (pBuffer.indexOf("SETDHCP=") >= 0)
  {
    Serial.print("Parametro:");
    Serial.println(pBuffer.substring(8));
    if (pBuffer.substring(9) == "ON")
    {
      SetDHCP(true);
      Serial.println("Ativou DHCP");
    }
    if (pBuffer.substring(9) == "OFF")
    {
      SetDHCP(false);
      Serial.println("Ativou DHCP");
    }
    flgRodou = true;
  }

  if (pBuffer.indexOf("CORR") >= 0)
  {
    //lcd.clear();
    Le_corr();
    //Serial.println("CORR");
    flgRodou = true;
  }
  
  
  if (pBuffer.indexOf("RELE01") >= 0)
  {
    //lcd.clear();
    Rele01();
    flgRodou = true;
  }
  if (pBuffer.indexOf("RELE01ON") >= 0)
  {
    //lcd.clear();
    Rele01ON();
    flgRodou = true;
  }
  if (pBuffer.indexOf("RELE01OFF") >= 0)
  {
    //lcd.clear();
    Rele01OFF();
    flgRodou = true;
  }
  
  if (pBuffer.indexOf("RELE02") >= 0)
  {
    //lcd.clear();
    Rele02();
    flgRodou = true;
  }
  
  if (pBuffer.indexOf("MAN") >= 0)
  {
    //lcd.clear();
    Help();
    //Serial.println("CORR");
    flgRodou = true;
  }
  if (pBuffer.indexOf("VER") >= 0)
  {
    Ver();
    flgRodou = true;
  }

  if (pBuffer.indexOf("IFCONFIG") >= 0)
  {
    //lcd.clear();
    IFConfig();
    //Serial.println("CORR");
    flgRodou = true;
  }

  if (flgRodou == false)
  {
    Println("Comando não reconhecido!");
  }
}

void ExecCMDClient(EthernetClient client, String pBuffer)
{
  bool flgRodou = false;


  //ZEROCORR
  if (pBuffer.indexOf("ZEROCORR") >= 0)
  {
    ZEROCORR();
    client.println("Zerou a corrente!");
    flgRodou = true;
  }

  //Serial.print(pBuffer.indexOf("MSG"));
  if (pBuffer.indexOf("SERIAL:") >= 0)
  {
    EnviaArduino(pBuffer.substring(5));
    flgRodou = true;
  }

  if (pBuffer.indexOf("SETMONITOR=") >= 0)
  {
    int Angulo = pBuffer.substring(6).toInt();
    Println(String(pBuffer.substring(6)));
    Println(String(pBuffer.substring(6).toInt()));
    if (pBuffer.substring(11) == "ON")
    {
      SETMONITOR(true);
    }
    if (pBuffer.substring(11) == "OFF")
    {
      SETMONITOR(false);
    }
    flgRodou = true;
  }

  if (pBuffer.indexOf("SETSSH=") >= 0)
  {
    client.print("Parametro:");
    client.println(pBuffer.substring(8));
    if (pBuffer.substring(8) == "ON")
    {
      SetSSH(true);
      client.println("Ativou SSH");
    }
    if (pBuffer.substring(8) == "OFF")
    {
      SetSSH(false);
      client.println("Desativou SSH");
    }
    flgRodou = true;
  }

  if (pBuffer.indexOf("SETDHCP=") >= 0)
  {
    client.print("Parametro:");
    client.println(pBuffer.substring(9));
    if (pBuffer.substring(9) == "ON")
    {
      SetDHCP(true);
      client.println("Ativou NTP");
    }
    if (pBuffer.substring(9) == "OFF")
    {
      SetDHCP(false);
      client.println("Desativou NTP");
    }
    flgRodou = true;
  }

  if (pBuffer.indexOf("CORR") >= 0)
  {
    //lcd.clear();
    Le_corrClient(client);
    //Serial.println("CORR");

    flgRodou = true;
  }
  
  if (pBuffer.indexOf("RELE01") >= 0)
  {
    //lcd.clear();
    Rele01();
    flgRodou = true;
  }
  
  if (pBuffer.indexOf("RELE01ON") >= 0)
  {
    Rele01ON();
    flgRodou = true;
  }
  if (pBuffer.indexOf("RELE01OFF") >= 0)
  {
    Rele01OFF();
    flgRodou = true;
  }
  
  if (pBuffer.indexOf("RELE02") >= 0)
  {
    //lcd.clear();
    Rele02();
    flgRodou = true;
  }
  
  if (pBuffer.indexOf("MAN") >= 0)
  {
    //lcd.clear();
    HelpClient(client);
    //Serial.println("CORR");
    flgRodou = true;
  }
  if (pBuffer.indexOf("VER") >= 0)
  {
    VerClient(client);
    flgRodou = true;
  }
  if (pBuffer.indexOf("STATUS") >= 0)
  {
    STATUSClient(client);
    flgRodou = true;
  }

  if (pBuffer.indexOf("IFCONFIG") >= 0)
  {
    //lcd.clear();
    IFConfigClient(client);
    //Serial.println("CORR");
    flgRodou = true;
  }

  if (flgRodou == false)
  {
    Println("Comando não reconhecido!");
  }
}

void SETMONITOR(bool Status)
{
  flgMonitor = Status;
}

//Le informação do webservice
void Le_EthernetSSH()
{
  // wait for a new client:
  clientssh = serverSSH.available();

  // when the client sends the first byte, say hello:
  if (clientssh)
  {
    if (!gotAMessage)
    {
      //Serial.print("Usuario se conectou:");

      WellcomeClient(clientssh);
      gotAMessage = true;
    }
    // read the bytes incoming from the client:
    inByte = clientssh.read();
    // echo the bytes back to the client:
    //server.write(thisChar);
    // echo the bytes to the server as well:
    //Serial.print(inByte);
    if (inByte != '\n')
    {
      Buffer += inByte;
    }
    else
    {
      ExecCMDClient(clientssh, Buffer);

      PromptClient(clientssh);
      Buffer = "";
    }

  }
}


void Le_Serial()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0)
  {
    // get incoming byte:
    inByte = Serial.read();
    //Serial.print(inByte); //Echo

    if (inByte != '\n')
    {
      Buffer += inByte; //Acumula no buffer da serial
      //Serial.print(Buffer);
    }
    else
    {
      Serial.print("Comando:");
      Serial.println(Buffer);
      ExecCMD(Buffer);
      Prompt();
      Buffer = ""; //Limpa buffer serial
    }
  }
}


//Leituras dos Dispositivos
void Leituras()
{
  Carrega_corr();
  Le_Serial(); //LE INFORMAÇÕES DA SERIAL
  Le_RF();
  //Le o flgSSH
  if (flgSSH)
  {
    Le_EthernetSSH(); //LE INFORMAÇÕES DA ETHERNET do servico SSH
  }
}


//Looping
void loop()
{
  Leituras();
}

