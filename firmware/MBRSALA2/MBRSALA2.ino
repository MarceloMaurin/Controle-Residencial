#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SD.h>
#include<stdlib.h>



#define DHTPIN 45    // what pin we're connected to
#define PinRele01 42  //Rele 1
#define PinRele02 44  //Rele 2
#define PinRele03 46  //Rele 3


// Uncomment whatever type you're using!
DHT dht;


float Temperatura = 0;
float Humidade = 0;

String Msg1, Msg2;

IPAddress ip(192, 168, 1, 177);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer srvMonitor(23);
boolean gotAMessage = false; // whether or not you got a message from the client yet


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns


//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {
    '1', '2', '3', 'A'
  }
  ,
  {
    '4', '5', '6', 'B'
  }
  ,
  {
    '7', '8', '9', 'C'
  }
  ,
  {
    '*', '0', '#', 'D'
  }
};

byte rowPins[ROWS] = {  25, 27, 29, 31}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 33, 35, 37, 39};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char customKey;


LiquidCrystal_I2C lcd(0x20, 16, 2); // set the LCD address to 0x20 for a 16 chars and 2 line display



/* Raw IR decoder sketch!

 This sketch/program uses the Arduno and a PNA4602 to
 decode IR received. This can be used to make a IR receiver
 (by looking for a particular code)
 or transmitter (by pulsing an IR LED at ~38KHz for the
 durations detected

 Code is public domain, check out www.ladyada.net and adafruit.com
 for more tutorials!
 */

// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
//uint8_t IRpin = 2;
// Digital pin #2 is the same as Pin D2 see
// http://arduino.cc/en/Hacking/PinMapping168 for the 'raw' pin mapping
#define IRpin_PIN      PIND
#define IRpin          2

//byte IRledPin =  3;    // LED connected to digital pin 3
byte IRledPin =  3;    // LED connected to digital pin 3
//byte ledPin =  34;     // LED connected to digital pin 13
byte ledPin =  40                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          ;     // LED connected to digital pin 13

byte RF01Pin =  A8;     //RF 01
byte RF02Pin =  A9;     //RF 02
byte RF03Pin =  A10;     //RF 03
byte RF04Pin =  A11;     //RF 04

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

String strIP = "";

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
File dataFile;
File currdir; //Diretorio corrente

//Tempo de Leitura
float tempo;
float timeslace;
float TEMPOLEITURAS = 5;
int faseMSG = 0;


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;

byte speakerPin = A12; //Speaker


// note names and their corresponding half-periods
byte names[] = {
  //'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'
};
int tones[] = {
  1915, 1700, 1519, 1432, 1275, 1136, 1014, 956
};


// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

float contciclo = 0;
char BufferKeypad[100];

//Versao do produto
char Versao = '20';
char Release = '2';

char NroSerie[20]  =  "110001001";

//prog_uchar SITE[] PROGMEM = {
char  SITE[80] =  "http://maurinsoft.com";
char AUTOR[100] = "Criado por Marcelo Maurin Martins";
//prog_uchar Produto[] PROGMEM = {
char Produto[10] = {"MBRSALA2"};

String Mensagem = "maurinsoft.com";

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2];  // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

void setup(void) {
  Start_Serial();
  Start_Bluetooth();
  Serial.println("Load firmware....");
  Serial2.println("Load firmware....");
  Start_lcd();
  Start_DTH();
  Speak_Start();
  Serial.println("Ativando reles");
  Serial2.println("Ativando reles");
  Rele_Start();
  Serial.println("Buscando SD Card");
  Serial2.println("Buscando SD Card");
  CLS();
  Imprime(0, "Buscando");
  Imprime(1, "SD Card");

  SD_Start();
  Serial.println("Buscando Ethernet");
  Serial2.println("Buscando Ethernet");
  CLS();
  Imprime(0, "Buscando");
  Imprime(1, "Rede");
  Start_Ethernet();

  Serial.println("Start outros servicos");
  Serial2.println("Start outros servicos");
  Start_IRLed();
  Start_Led();
  Serial.println("Start web service");
  Serial2.println("Start web service");
  CLS();
  Imprime(0, "Start");
  Imprime(1, "Webservice");
  Start_Server();

  WellCome();
  Sound('a');
  Sound('d');

  //Inicia Contagem
  timeslace = millis();
}


void Start_DTH()
{
  dht.setup(DHTPIN);
  Humidade = dht.getHumidity();
  Temperatura = dht.getTemperature();
  if (isnan(Humidade) || isnan(Temperatura))
  {
    CLS();
    Imprime(0, "Erro");
    Imprime(1, "DHT nao iniciou");
    Serial.println("DHT não iniciou");
    Serial2.println("DHT não iniciou");
    delay(2000);
  }
}


void Start_lcd()
{
  lcd.init();                      // initialize the lcd
  lcd.backlight();

}

void Rele_Start()
{
  Serial.println("Rele Starting...");
  pinMode(PinRele01, OUTPUT);
  pinMode(PinRele02, OUTPUT);
  pinMode(PinRele03, OUTPUT);
  digitalWrite(PinRele02, LOW);
  digitalWrite(PinRele01, LOW);
  digitalWrite(PinRele03, LOW);

  Serial.println("Rele OK");
  Serial2.println("Rele OK");
}

void SD_Start()
{
  Serial.println("Iniciando cartão SD");
  Serial2.println("Iniciando cartão SD");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(53, OUTPUT);     // change this to 53 on a mega
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");

    Serial2.println("initialization failed. Things to check:");
    Serial2.println("* is a card is inserted?");
    Serial2.println("* Is your wiring correct?");
    Serial2.println("* did you change the chipSelect pin to match your shield or module?");

    return;
  }


  // print the type of card
  Serial.print("\nCard type: ");
  Serial2.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      Serial2.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      Serial2.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      Serial2.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
      Serial2.println("Unknown");
  }

}




void DISKFREE()
{

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    Serial2.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial2.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial2.println(volume.fatType(), DEC);
  Serial.println();
  Serial2.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  volumesize /= 1024;

  Serial.print("Volume size (Mbytes): ");
  Serial2.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial2.println(volumesize);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  Serial2.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  /*
  // list all files in the card with date and size
   root.ls(LS_R | LS_DATE | LS_SIZE);
   */

}


//LS de diretorio corrente
void LS(int numTabs)
{
  while (true) {

    File entry =  currdir.openNextFile(); //diretorio corrente
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
      Serial2.print('\t');
    }
    Serial.print(entry.name());
    Serial2.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      Serial2.println("/");
      LS(numTabs + 1);
    }
    else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
      Serial2.print("\t\t");
      Serial2.println(entry.size(), DEC);

    }
    entry.close();
  }
}

//Gera um arquivo de Log
void LogRec(String Info)
{
  char Filename[20];
  sprintf(Filename, "logrec.txt");
  if (!SD.exists(Filename))
  {
    dataFile = SD.open(Filename, FILE_WRITE);
    dataFile.close();
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open(Filename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(Info);
  }
  // if the file isn't open, pop up an error:
  else
  {
    //Serial.println("error opening datalog.txt");
    //CLS();
    //Imprime(0,"SD Erro");
    //Imprime(1,"Ocorrencia N Grav");
    Serial.println("SD ERRO, Ocorrencia N Gravada");
    Serial2.println("SD ERRO, Ocorrencia N Gravada");
    Sound('d');
  }
  dataFile.close();
}


//Inicializa Speaker
void Speak_Start()
{
  Serial.println("Speaker Starting...");
  Serial2.println("Speaker Starting...");
  pinMode(speakerPin, OUTPUT);
  Serial.println("Speaker OK");
  Serial2.println("Speaker OK");
}


void CLS()
{
  lcd.clear();
}

//Imprime linha
void Imprime(int y, String Info)
{
  lcd.setCursor(0, y);
  lcd.print(Info);

}

//Toca um som
void Sound(char serByte)
{
  int count = 0;

  for (count = 0; count <= 8; count++) { // look for the note
    if (names[count] == serByte) {  // ahh, found it
      for ( int i = 0; i < 20; i++ ) { // play it for 50 cycles
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tones[count]);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tones[count]);
      }
    }
  }
}



void Le_Teclado()
{
  //Le Teclado
  customKey = customKeypad.getKey();
  if (customKey) {

    if (customKey >= '0' && customKey <= '9')
    {
      sprintf(BufferKeypad, "%s%c", BufferKeypad, customKey);
      //sprintf(BufferKeypad"%s%c",BufferKeypad,customKey);
      //Serial.println(customKey);
      CLS();
      Imprime(0, "KEYBOARD:");
      Imprime(1, BufferKeypad);
      Sound('d');

    }
    if (customKey == '#') //Limpa Buffer
    {
      sprintf(BufferKeypad, "%s\n", BufferKeypad);
      //BufferKeypad += '\n';
      Sound('e');
    }
    if (customKey == '*') //Limpa Buffer
    {
      sprintf(BufferKeypad, "");
      //BufferKeypad = "";
      Sound('f');

      CLS();
      Imprime(0, "KEYBOARD:");
      Imprime(1, BufferKeypad);
      Sound('a');
    }
    //Macro Funções
    if (customKey == 'A') //LUZ
    {
      sprintf(BufferKeypad, "KEYA\n");
      //BufferKeypad = "LUZ\n";

    }
    //Macro Funções
    if (customKey == 'B') //Abre Porta
    {
      sprintf(BufferKeypad, "KEYB\n");
      //BufferKeypad = "PORTA\n";

    }
    //Macro Funções
    if (customKey == 'C') //TRAVA
    {
      sprintf(BufferKeypad, "KEYC\n");
      //BufferKeypad = "TV\n";

    }
    //Macro Funções
    if (customKey == 'D') //LIGAR
    {
      sprintf(BufferKeypad, "KEYD\n");
    }
  }
}


void Start_Ethernet()
{
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha ao pesquisar DHCP");
    Serial2.println("Falha ao pesquisar DHCP");
    // no point in carrying on, so do nothing forevermore:
    //for(;;)
    //  ;
    strIP = "192.168.0.177";
    Ethernet.begin(mac, ip, gateway, subnet);
  }
  else
  {
    strIP = "";
  }
  //String strIP = "";
  // print your local IP address:
  Serial.print("DHCP Server find, IP: ");
  Serial2.print("DHCP Server find, IP: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial2.print(Ethernet.localIP()[thisByte], DEC);
    strIP = strIP + String(Ethernet.localIP()[thisByte], DEC) + ".";
    Serial.print(".");
    Serial2.print(".");
  }
  CLS();
  Imprime(0, "IP encontrado");
  Imprime(1, strIP);
  delay(2000);
  Serial.println();
  Serial2.println();

}

//Start Servidor
void Start_Server()
{
  srvMonitor.begin();
}

//Aciona TV
void PowerRF()
{
  CLS();
  Imprime(0, Produto);
  Imprime(1, "Power IR");
  delayMicroseconds(45288);
  pulseIR(8920);
  delayMicroseconds(4320);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(600);
  delayMicroseconds(1580);
  pulseIR(660);
  delayMicroseconds(440);
  pulseIR(640);
  delayMicroseconds(440);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(620);
  delayMicroseconds(480);
  pulseIR(620);
  delayMicroseconds(1560);
  pulseIR(660);
  delayMicroseconds(1540);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(640);
  delayMicroseconds(1540);
  pulseIR(660);
  delayMicroseconds(1540);
  pulseIR(660);
  delayMicroseconds(1540);
  pulseIR(640);
  delayMicroseconds(1560);
  pulseIR(640);
  delayMicroseconds(1560);
  pulseIR(640);
  delayMicroseconds(440);
  pulseIR(660);
  delayMicroseconds(440);
  pulseIR(620);
  delayMicroseconds(480);
  pulseIR(620);
  delayMicroseconds(1560);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(620);
  delayMicroseconds(480);
  pulseIR(660);
  delayMicroseconds(420);
  pulseIR(680);
  delayMicroseconds(1520);
  pulseIR(640);
  delayMicroseconds(1540);
  pulseIR(660);
  delayMicroseconds(1540);
  pulseIR(640);
  delayMicroseconds(460);
  pulseIR(640);
  delayMicroseconds(1560);
  pulseIR(640);
  delayMicroseconds(1540);
  pulseIR(660);
  delayMicroseconds(1540);
  pulseIR(640);
  delayMicroseconds(1560);
  pulseIR(640);
  delayMicroseconds(39320);
  pulseIR(8920);
  delayMicroseconds(2120);
  pulseIR(640);
  delayMicroseconds(28764);
  pulseIR(8900);
  delayMicroseconds(2120);
  pulseIR(660);
}

//Recebe dados do controle remoto
void RFReceive()
{
  uint16_t highpulse, lowpulse;  // temporary storage timing
  highpulse = lowpulse = 0; // start out with no pulse length

  //  while (digitalRead(IRpin)) { // this is too slow!
  while (IRpin_PIN & _BV(IRpin))
  {
    // pin is still HIGH

    // count off another few microseconds
    highpulse++;
    delayMicroseconds(RESOLUTION);

    // If the pulse is too long, we 'timed out' - either nothing
    // was received or the code is finished, so print what
    // we've grabbed so far, and then reset
    if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
      printpulses();
      currentpulse = 0;
      return;
    }
  }
  // we didn't time out so lets stash the reading
  pulses[currentpulse][0] = highpulse;

  // same as above
  while (! (IRpin_PIN & _BV(IRpin))) {
    // pin is still LOW
    lowpulse++;
    delayMicroseconds(RESOLUTION);
    if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
      printpulses();
      currentpulse = 0;
      return;
    }
  }
  pulses[currentpulse][1] = lowpulse;

  // we read one high-low pulse successfully, continue!
  currentpulse++;
}

void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  Serial2.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print("delayMicroseconds(");
    Serial2.print("delayMicroseconds(");
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial2.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.println(");");
    Serial2.println(");");
    Serial.print("pulseIR(");
    Serial2.print("pulseIR(");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial2.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(");");
    Serial2.println(");");
  }
}


//Captura informações de Temperatura
void Dht11()
{
  //Somente faz se flgVozRoute,pois este faz controle de ROTEAMENTO
  Humidade = dht.getHumidity();
  Temperatura = dht.getTemperature();


}


void Start_Bluetooth()
{
  Serial2.begin(9600);

}

void Start_Serial()
{
  Serial.begin(57600);

}

void Start_IRLed()
{
  pinMode(IRledPin, OUTPUT);
  digitalWrite(IRledPin, HIGH);
}

void Start_Led()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

byte sendEmail()
{
  //char Empresa[200] =  "Maurinsoft automacao inteligente para sua casa";
  // change server to your email server ip or domain
  //IPAddress server(186,202,153,149);
  char server[] = "smtp.site1367007914.provisorio.ws";
  //char EMAIL[80] =  "comercial@maurinsoft.com";


  CLS();
  Imprime(0, "Acionamento detectado");
  Imprime(1, "Enviando Email");
  byte thisByte = 0;
  byte respCode;
  Serial.println(F("Enviando email..."));
  Serial2.println(F("Enviando email..."));
  if (client.connect(server, 25)) {
    Serial.println(F("connected"));
    Serial2.println(F("connected"));
  }
  else {
    Serial.println(F("connection failed"));
    Serial2.println(F("connection failed"));
    CLS();
    Imprime(0, "Falha");
    Imprime(1, "Envio de Email");
    return 0;
  }

  if (!eRcv()) return 0;
  Serial.println(F("Sending helo"));
  Serial2.println(F("Sending helo"));

  // change to your public ip
  client.println(F("helo site1367007914.provisorio.ws"));

  if (!eRcv()) return 0;
  Serial.println(F("Sending From"));
  Serial2.println(F("Sending From"));

  // change to your email address (sender)
  client.println(F("MAIL From: marcelomaurinmartins@maurinsoft.com.br"));

  if (!eRcv()) return 0;

  // change to recipient address
  Serial.println(F("Sending To"));
  Serial2.println(F("Sending To"));
  client.println(F("RCPT To: marcelomaurinmartins@gmail.com"));

  if (!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  Serial2.println(F("Sending DATA"));
  client.println(F("DATA"));

  if (!eRcv()) return 0;

  Serial.println(F("Sending email"));
  Serial2.println(F("Sending email"));

  // change to recipient address
  client.println(F("To: Marcelo marcelomaurinmartins@gmail.com"));

  // change to your address
  client.println(F("From: marcelo marcelomaurinmartins@maurinsoft.com"));

  client.println(F("Subject: Arduino email test\r\n"));

  client.println(F("This is from my Arduino!"));

  client.println(F("."));

  if (!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  Serial2.println(F("Sending QUIT"));
  client.println(F("QUIT"));

  if (!eRcv()) return 0;

  client.stop();

  Serial.println(F("disconnected"));
  Serial2.println(F("disconnected"));
  CLS();
  Imprime(0, "Sucesso");
  Imprime(1, "Envio de Email");

  return 1;
}
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      Serial2.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();

  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
    Serial2.write(thisByte);
  }

  if (respCode >= '4')
  {
    efail();
    return 0;
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  client.println(F("QUIT"));

  while (!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      Serial2.println(F("\r\nTimeout"));
      return;
    }
  }

  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
    Serial2.write(thisByte);
  }

  client.stop();

  Serial.println(F("disconnected"));
  Serial2.println(F("disconnected"));
}


void WellCome()
{
  Serial.println(" ");
  Serial.println("Bem vindo");
  Serial2.println(" ");
  Serial2.println("Bem vindo");

  //Serial.println(Empresa);
  Serial.println("MBRSALA2 - Controle Residencial 11");
  Serial.println("MBRSALA2 - Controle Residencial 11");

  Serial.print("Produto:");
  Serial.println(Produto);

  Serial2.print("Produto:");
  Serial2.println(Produto);

  Serial.print("Versao:");
  Serial.print(Versao);
  Serial2.print("Versao:");
  Serial2.print(Versao);

  Serial.print(".");
  Serial.println(Release);

  Serial2.print(".");
  Serial2.println(Release);

  Serial.print("site:");
  Serial.println(SITE);

  Serial2.print("site:");
  Serial2.println(SITE);

  Serial.print("email:");
  //Serial.println(EMAIL);

  Serial2.print("email:");
  //Serial2.println(EMAIL);

  Serial.println(AUTOR);
  Serial2.println(AUTOR);

  Serial.println(" ");
  Serial2.println(" ");
  if (client)
  {
    client.println(" ");
    client.println("Bem vindo");

    //Serial.println(Empresa);
    client.println("MBR11 - Controle Residencial 11");

    client.print("Produto:");
    client.println(Produto);

    client.print("Versao:");
    client.print(Versao);
    client.print(".");
    client.println(Release);

    client.print("site:");
    client.println(SITE);

    client.print("email:");
    //Serial.println(EMAIL);

    client.println(AUTOR);

    client.println(" ");
  }
  CLS();
  Imprime(0, Produto);
  Imprime(1, "Casa Monitorada");
  Imprime();
}



// This procedure sends a 38KHz pulse to the IRledPin
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait

  cli();  // this turns off any background interrupts

  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    digitalWrite(IRledPin, LOW);  // this takes about 3 microseconds to happen
    delayMicroseconds(10);         // hang out for 10 microseconds
    digitalWrite(IRledPin, HIGH);   // this also takes about 3 microseconds
    delayMicroseconds(10);         // hang out for 10 microseconds

    // so 26 microseconds altogether
    microsecs -= 26;
  }

  sei();  // this turns them back on
}

void Le_Led()
{
  byte led = digitalRead(ledPin);
  if (led == HIGH)
  {
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(ledPin, HIGH);
  }
  //digitalWrite(IRledPin, LOW);
}

int Le_RF01()
{
  // read the analog in value:
  sensorValue = analogRead(RF01Pin);
  // map it to the range of the analog out:
  return map(sensorValue, 0, 1023, 0, 255);
}

int Le_RF02()
{
  // read the analog in value:
  sensorValue = analogRead(RF02Pin);
  // map it to the range of the analog out:
  return map(sensorValue, 0, 1023, 0, 255);
}

int Le_RF03()
{
  // read the analog in value:
  sensorValue = analogRead(RF03Pin);
  // map it to the range of the analog out:
  return map(sensorValue, 0, 1023, 0, 255);
}

int Le_RF04()
{
  // read the analog in value:
  sensorValue = analogRead(RF04Pin);
  // map it to the range of the analog out:
  return map(sensorValue, 0, 1023, 0, 255);

}


void Le_RF()
{
  //Serial.println(Le_RF01());
  if (Le_RF01() >= 250)
  {
    sprintf(BufferKeypad, "KEYA\n");
  }
  if (Le_RF02() >= 250)
  {
    sprintf(BufferKeypad, "KEYB\n");
  }
  if (Le_RF03() >= 250)
  {
    sprintf(BufferKeypad, "KEYC\n");
  }
  if (Le_RF04() >= 250)
  {
    sprintf(BufferKeypad, "KEYD\n");
  }

}

void Le_Server()
{

  // wait for a new client:
  EthernetClient client = srvMonitor.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!gotAMessage) {
      Serial.println("Conexão estabelecida");
      Serial2.println("Conexão estabelecida");
      client.println(String(Produto) + String(" - Bem vindo"));
      gotAMessage = true;
    }

    // read the bytes incoming from the client:
    char thisChar = client.read();
    if (thisChar != 0)
    {
      //BufferKeypad = sprintf("%s%c",BufferKeypad,thisChar);,
      sprintf(BufferKeypad, "%s%c", BufferKeypad, thisChar);
    }
    // echo the bytes back to the client:
    srvMonitor.write(thisChar);
    // echo the bytes to the server as well:
    Serial.print(thisChar);
    Serial2.print(thisChar);
  }
}

//Le registro da Serial
void Le_Serial()
{
  char key;
  while (Serial.available() > 0)
  {
    key = Serial.read();

    if (key != 0)
    {
      Serial.print(key);
      //BufferKeypad += key;
      sprintf(BufferKeypad, "%s%c", BufferKeypad, key);
    }
  }
}

//Le registro do bluetooth
void Le_Bluetooth()
{
  while (Serial2.available() > 0)
  {
    char key = Serial2.read();
    if (key != 0)
    {
      Serial2.print(key);
      //BufferKeypad += key;
      sprintf(BufferKeypad, "%s%c", BufferKeypad, key);
    }
  }
}


void Leituras()
{
  Le_RF();
  Le_Teclado();
  Le_Serial(); //Le dados do Serial
  Le_Bluetooth(); //Le Dados do Bluetooth
  Le_Server();
  KeyCMD();

  //Captura o tempo passado
  tempo =  (millis() - timeslace) / 1000;
  if (tempo > TEMPOLEITURAS)
  {
    switch (faseMSG)
    {
      case 0 : {
          //Le temperatura
          Temp();
          faseMSG++;
          break;

        }
      case 1 : { //Produto
          CLS();
          Imprime(0, Produto);
          Imprime(1, String(Versao) + '.' + String(Release));
          faseMSG++;
          break;
        }
      case 2 : {
          CLS();
          Imprime(0, Msg1);
          Imprime(1, Msg2);
          faseMSG++;
          break;
        }
      case 3 : {
          CLS();
          Imprime(0, "Hora HH:MM");
          Imprime(1, "Data DD/MM/YY");
          faseMSG++;
          break;
        }
      case 4 : {
          CLS();
          Imprime(0, "Servidor");
          Imprime(1, "ONLINE");
          faseMSG++;
          faseMSG = 0;
          break;
        }
    }

    timeslace = millis();
  }
  if (contciclo > 2000)
  {
    Dht11();
    Le_Led();
    contciclo = 0;
  }
  contciclo++;
}



void Temp()
{
  Dht11();

  CLS();
  char Buffer[20];
  dtostrf(Humidade, 6, 2, Buffer);
  String Info = "Humid.:" + String(Buffer);
  //dtostrf(FLOAT,WIDTH,PRECSISION,BUFFER);
  Imprime(0, Info);
  Serial.print('Humidade:');
  Serial.println(Buffer);
  Serial2.print('Humidade:');
  Serial2.println(Buffer);
  dtostrf(Temperatura, 6, 2, Buffer);
  Info = "Temp.:" + String(Buffer);
  Imprime(1, Info);
  Serial.print('Temperatura:');
  Serial.println(Buffer);
  Serial2.print('Temperatura:');
  Serial2.println(Buffer);
  //Sound('a');
}

//Atualiza informação do Rela02
void AtualizaRele01()
{
  if (digitalRead(PinRele01) == true)
  {
    //byte value = digitalRead(Rele01Pin);
    digitalWrite(PinRele01, LOW);    //liga o LED da porta 13
  }
  else
  {

    digitalWrite(PinRele01, HIGH);    //liga o LED da porta 13
  }
  delay(1000);//Demora para sentir mudança
}

//Atualiza informação do Rela02
void AtualizaRele02()
{
  if (digitalRead(PinRele02) == true)
  {
    //byte value = digitalRead(Rele01Pin);
    digitalWrite(PinRele02, LOW);    //liga o LED da porta 13
  }
  else
  {

    digitalWrite(PinRele02, HIGH);    //liga o LED da porta 13
  }
  delay(1000);
}

//Atualiza informação do Rela03
void AtualizaRele03()
{
  if (digitalRead(PinRele03) == true)
  {
    //byte value = digitalRead(Rele01Pin);
    digitalWrite(PinRele03, LOW);    //liga o LED da porta 13
  }
  else
  {

    digitalWrite(PinRele03, HIGH);    //liga o LED da porta 13
  }
  delay(1000);
}



//Implementa comandos de KEY (Funções macro
void KEY(int Funcao)
{
  switch (Funcao)
  {
    case 0 : //Função 1 - Liga o Rele
      {
        AtualizaRele01();
        break;
      }
    case 1 : //Funcao 2 - Aciona a TV e o Blueray
      {
        //PowerRF();
        AtualizaRele02();
        //Ainda nao implementado
        break;
      }
    case 2 : //Mede a temperatura
      {
        //Temp();
        AtualizaRele03();
        break;
      }
    case 3 : //Chama o alarme do Servidor
      {
        Temp();
        break;
      }
  }
}

//Comando de entrada do Teclado
void KeyCMD()
{
  bool resp = false;

  //incluir busca /n

  if (strchr (BufferKeypad, '\n') != 0)
  {
    if (strcmp( BufferKeypad, "DESLIGAR\n") == 0)
    {
      PowerRF();
      resp = true;
    }
    if (strcmp( BufferKeypad, "10\n") == 0)
    {
      PowerRF();
      resp = true;
    }
    
    if (strcmp( BufferKeypad, "11\n") == 0)
    {
      Temp();
      resp = true;
    }
    
    if (strcmp( BufferKeypad, "12\n") == 0)
    {
      RFReceive();
      resp = true;
    }
    if (strcmp( BufferKeypad, "13\n") == 0)
    {
      AtualizaRele01();
      resp = true;
    }

    if (strcmp( BufferKeypad, "14\n") == 0)
    {
      AtualizaRele02();
      resp = true;
    }

    if (strcmp( BufferKeypad, "15\n") == 0)
    {
      AtualizaRele03();
      resp = true;
    }



    if (strcmp( BufferKeypad, "LIGAR\n") == 0)
    {
      PowerRF();
      resp = true;
    }
    if (strcmp( BufferKeypad, "PROGRAMAR\n") == 0)
    {
      RFReceive();
      resp = true;
    }

    if (strcmp( BufferKeypad, "TEMP\n") == 0)
    {
      Temp();
      resp = true;
    }

    if (strcmp( BufferKeypad, "KEYA\n") == 0)
    {
      KEY(0);
      resp = true;
    }
    if (strcmp( BufferKeypad, "KEYB\n") == 0)
    {
      KEY(1);
      resp = true;
    }

    if (strcmp( BufferKeypad, "KEYC\n") == 0)
    {
      KEY(2);
      resp = true;
    }
    if (strcmp( BufferKeypad, "KEYD\n") == 0)
    {
      KEY(3);
      resp = true;
    }

    if (strcmp( BufferKeypad, "MAN\n") == 0)
    {
      Man();
      resp = true;
    }

    if (resp == false)
    {
      Serial.print("Comando:");
      Serial2.print("Comando:");
      Serial.print(BufferKeypad);
      Serial2.print(BufferKeypad);
      Serial.println("Comando nao reconhecido!");
      Serial2.println("Comando nao reconhecido!");
      //strcpy(BufferKeypad,'\0');
      memset(BufferKeypad, 0, sizeof(BufferKeypad));
    }
    else
    {
      //strcpy(BufferKeypad,'\0');
      memset(BufferKeypad, 0, sizeof(BufferKeypad));
    }

    Imprime();
    //Imprime(DevSerial);

  }
}


void Man()
{
  Serial.println(" ");
  Serial.println("MAN - manual do equipamento");
  Serial.println("DESLIGAR - desliga equipamento");
  Serial.println("LIGAR - estado normal do equipamento");
  Serial.println("PROGRAMAR - Estado de programacao de IR");
  Serial.println("Temp - Le temperatura da sala");
  Serial.println("KEYA - Aciona macro A");
  Serial.println("KEYB - Aciona macro B");
  Serial.println("KEYC - Aciona macro C");
  Serial.println("KEYD - Aciona macro D");

  if (client)
  {
    client.println(" ");
    client.println("MAN - manual do equipamento");
    client.println("DESLIGAR - desliga equipamento");
    client.println("LIGAR - estado normal do equipamento");
    client.println("PROGRAMAR - Estado de programacao de IR");
    client.println("TEMP - Le temperatura da sala");
    client.println("KEYA - Aciona macro A");
    client.println("KEYB - Aciona macro B");
    client.println("KEYC - Aciona macro C");
    client.println("KEYD - Aciona macro D");
  }
  Serial2.println(" ");
  Serial2.println("MAN - manual do equipamento");
  Serial2.println("DESLIGAR - desliga equipamento");
  Serial2.println("LIGAR - estado normal do equipamento");
  Serial2.println("PROGRAMAR - Estado de programacao de IR");
  Serial2.println("Temp - Le temperatura da sala");
  Serial2.println("KEYA - Aciona macro A");
  Serial2.println("KEYB - Aciona macro B");
  Serial2.println("KEYC - Aciona macro C");
  Serial2.println("KEYD - Aciona macro D");
}

//Imprime padrão demonstrando entrada de comando
void Imprime()
{
  //Serial.println(" ");
  Serial.print("/>");
  if (client)
  {
    client.print("/>");
  }
  Serial2.print("/>");
}

//Roda as rotinas
void loop()
{
  Leituras();
  //Serial.print(".");
  //delayMicroseconds(1000);
}





