
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include<stdlib.h>




#define PinRele01 39  //Rele 1
#define PinRele02 41  //Rele 2
#define PinRele03 43  //Rele 3

#define PinAgua01 45  //sensor de agua01
#define PinAgua02 47  //sensor de agua02
#define PinAnalAgua01 A0  //sensor de agua01
#define PinAnalAgua02 A1  //sensor de agua02
#define ledPin   13      // LED connected to digital pin 13



String Msg1, Msg2;




const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns


//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {
    'A', 'B', '#', '*'
  }
  ,
  {
    '1', '2', '3', '^'
  }
  ,
  {
    '4', '5', '6', 'v'
  }
  ,

  {
    '7', '8', '9', 'E'
  }
  ,
  {
    '<', '0', '>', 'C'
  }
};

byte rowPins[ROWS] = {  28, 30, 32, 34, 36}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 44, 42, 40, 38};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char customKey;


LiquidCrystal_I2C lcd(0x20, 20, 4); // set the LCD address to 0x20 for a 16 chars and 2 line display





//Tempo de Leitura
float tempo;
float timeslace;
float TEMPOLEITURAS = 5;
int faseMSG = 0;



// note names and their corresponding half-periods
byte names[] = {
  //'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'
};

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

float contciclo = 0;
char BufferKeypad[100];

//Versao do produto
char Versao = '12';
char Release = '3';

char NroSerie[20]  =  "120001001";

//prog_uchar SITE[] PROGMEM = {
char  SITE[80] =  "http://maurinsoft.com";
char AUTOR[100] = "Criado por Marcelo Maurin Martins";
//prog_uchar Produto[] PROGMEM = {
char Produto[10] = {"MBRIRRIGA"};

String Mensagem = "maurinsoft.com";

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2];  // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

void setup(void)
{
  Start_Serial();
  Start_Bluetooth();
  Serial.println("Load firmware....");
  Serial2.println("Load firmware....");
  Start_lcd();
  CLS();
  Imprime(0, "Buscando");
  Imprime(1, "SD Card");

  Serial.println("Ativando reles");
  Serial2.println("Ativando reles");
  Rele_Start();
  Serial.println("Buscando SD Card");
  Serial2.println("Buscando SD Card");

  // SD_Start();
  Serial.println("Buscando Ethernet");
  Serial2.println("Buscando Ethernet");
  CLS();
  Imprime(0, "Buscando");
  Imprime(1, "Rede");
  //Start_Ethernet();
  Serial.println("Start Sensor de Agua");
  Agua_Start();

  Serial.println("Start outros servicos");
  Serial2.println("Start outros servicos");

  Start_Led();


  WellCome();
  delay(2000);
  //Inicia Contagem
  timeslace = millis();
}


void Start_lcd()
{
  lcd.init();                      // initialize the lcd
  lcd.begin(20, 4);
  lcd.backlight();
  CLS();

}

void Rele_Start()
{
  Serial.println("Rele Starting...");
  pinMode(PinRele01, OUTPUT);
  pinMode(PinRele02, OUTPUT);
  pinMode(PinRele03, OUTPUT);
  digitalWrite(PinRele02, HIGH);
  digitalWrite(PinRele01, HIGH);
  digitalWrite(PinRele03, HIGH);

  Serial.println("Rele OK");
  Serial2.println("Rele OK");
}

void Agua_Start()
{
  pinMode(PinAgua01, INPUT);
  pinMode(PinAgua02, INPUT);
  pinMode(PinAnalAgua01,INPUT);
  pinMode(PinAnalAgua02,INPUT);
  Serial.println("Agua OK");
  Serial2.println("Agua OK");
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

void Le_Teclado()
{
  //Le Teclado
  customKey = customKeypad.getKey();
  if (customKey)
  {

    if (customKey >= '0' && customKey <= '9')
    {
      sprintf(BufferKeypad, "%s%c", BufferKeypad, customKey);
      //sprintf(BufferKeypad"%s%c",BufferKeypad,customKey);
      //Serial.println(customKey);
      CLS();
      Imprime(0, "KEYBOARD:");
      Imprime(1, BufferKeypad);
    }
    if (customKey == '#') //Limpa Buffer
    {
      sprintf(BufferKeypad, "%s\n", BufferKeypad);
      //BufferKeypad += '\n';
    }
    if (customKey == '*') //Limpa Buffer
    {
      sprintf(BufferKeypad, "");
      //BufferKeypad = "";

      CLS();
      Imprime(0, "KEYBOARD:");
      Imprime(1, BufferKeypad);
    }


    //Macro Funções
    if (customKey == 'C') //Enter
    {
      sprintf(BufferKeypad, "%s\n", BufferKeypad);//Enter ao final

    }
    //Macro Funções
    if (customKey == 'E') //ESQ limpa buffer
    {
      memset(BufferKeypad, 0, sizeof(BufferKeypad));
      //BufferKeypad = "";

      CLS();
      Imprime(0, "KEYBOARD:");
      Imprime(1, BufferKeypad);
    }
  }

}


void Start_Bluetooth()
{
  Serial2.begin(9600);

}

void Start_Serial()
{
  Serial.begin(57600);

}



void Start_Led()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}



void WellCome()
{
  Serial.println(" ");
  Serial.println("Bem vindo");
  Serial2.println(" ");
  Serial2.println("Bem vindo");

  //Serial.println(Empresa);
  Serial.println("MBRIRRIGA - Controle Residencial 12");
  Serial.println("MBRIRRIGA - Controle Residencial 12");

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

  CLS();
  Imprime(0, Produto);
  Imprime(1, "Casa Monitorada");
  Prompt();
}


void Le_Led()
{
  byte led = digitalRead(ledPin);
  if (led == HIGH)
  {
    digitalWrite(ledPin, LOW);
  }
  else 
  {
    digitalWrite(ledPin, HIGH);
  }
  //digitalWrite(IRledPin, LOW);
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

void Le_Agua()
{
  byte Agua01 =  digitalRead(PinAgua01);
  byte Agua02 =  digitalRead(PinAgua02);
  if ((Agua01 == HIGH) && (Agua02 == HIGH))
  {
    AcionaIrrigacao();
  }
}

void AcionaIrrigacao()
{
  CLS();
  Imprime(0,"Irrigação Acionada");
  Serial.println("Rele01 acionado");
  digitalWrite(PinRele01, LOW);
  delay(12000);
  Imprime(0,"Irrigação Desligada");
  digitalWrite(PinRele01, HIGH);
  Serial.println("Rele01 Desligado");
}


void Leituras()
{
  Le_Teclado();
  Le_Serial(); //Le dados do Serial
  Le_Bluetooth(); //Le Dados do Bluetooth

  Le_Agua();
  KeyCMD();

  //Captura o tempo passado
  tempo =  (millis() - timeslace) / 1000;
  if (tempo > TEMPOLEITURAS)
  {
    switch (faseMSG)
    {
      case 0 : {
          //Le temperatura
          //Temp();
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
          //faseMSG++;
          faseMSG = 0;
          break;
        }
    }

    timeslace = millis();
  }
  if (contciclo > 1000)
  {
    //AcionaIrrigacao();
    Le_Led();
    contciclo = 0;
  }
  contciclo++;
}




//Atualiza informação do Rela02
void AtualizaRele01()
{

  if (digitalRead(PinRele01) == true)
  {
    //byte value = digitalRead(Rele01Pin);
    //digitalWrite(PinRele01, LOW);    //liga o LED da porta 13
  }
  else
  {

    //digitalWrite(PinRele01, HIGH);    //liga o LED da porta 13
  }
  //delay(1000);//Demora para sentir mudança

}

//Atualiza informação do Rela02
void AtualizaRele02()
{
  if (digitalRead(PinRele02) == true)
  {
    //byte value = digitalRead(Rele01Pin);
    //digitalWrite(PinRele02, LOW);    //liga o LED da porta 13
  }
  else
  {

    //digitalWrite(PinRele02, HIGH);    //liga o LED da porta 13
  }
  //delay(1000);
}

//Atualiza informação do Rela03
void AtualizaRele03()
{
  if (digitalRead(PinRele03) == true)
  {
    //byte value = digitalRead(Rele01Pin);
    //digitalWrite(PinRele03, LOW);    //liga o LED da porta 13
  }
  else
  {
    //digitalWrite(PinRele03, HIGH);    //liga o LED da porta 13
  }
  //delay(1000);
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
      //PowerRF();
      resp = true;
    }

    if (strcmp( BufferKeypad, "LIGAR\n") == 0)
    {
      //PowerRF();
      resp = true;
    }


    if (strcmp( BufferKeypad, "AGUA\n") == 0)
    {
      AcionaIrrigacao();
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

    Prompt();
    //Imprime(DevSerial);

  }
}


void Man()
{
  Serial.println(" ");
  Serial.println("MAN - manual do equipamento");
  Serial.println("DESLIGAR - desliga equipamento");
  Serial.println("LIGAR - estado normal do equipamento");
  Serial.println("AGUA - Ativa irrigação de forma manual");


  Serial2.println(" ");
  Serial2.println("MAN - manual do equipamento");
  Serial2.println("DESLIGAR - desliga equipamento");
  Serial2.println("LIGAR - estado normal do equipamento");
  Serial2.println("AGUA - Ativa irrigação de forma manual");
}

//Imprime padrão demonstrando entrada de comando
void Prompt()
{
  //Serial.println(" ");
  Serial.print("/>");

  Serial2.print("/>");
}

//Roda as rotinas
void loop()
{
  Leituras();
  //Serial.print(".");
  //delayMicroseconds(1000);
}





