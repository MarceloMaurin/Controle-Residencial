
#include <SD.h>
#include <SPI.h>





String strversao  = "1.23";


// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
boolean SdOk;
boolean SdMount;
boolean resultado;
int Bot = 0;
int info = 0;
// print the type and size of the first FAT-type volume
uint32_t volumesize;



// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;   

//Sensor de presença
const byte sensorp = 12;
const byte piscaled = 13;

String inputString;         // a string to hold incoming data
boolean stringComplete;  // whether the string is complete






boolean gotAMessage = false; // whether or not you got a message from the client yet





//Impressão padronizada
void Println(int Tipo, String Msg)
{
  
  //Tipo 
  // 0 - Serial
  // 1 - Client ethernet
  if (Tipo == 0)
  {
    Serial.println(Msg);
  }

}


//Configura Cartao SD
boolean SetupSD(int Tipo)
{
  boolean resultado = false;
  Println(Tipo,"Verificando cartão SD:");
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Println(Tipo,"Confirme se cartão SD esta conectado no slot");
  }
    else 
  {
    Println(Tipo,"Cartão SD esta corretamente montado."); 
    resultado = true;
  } 
  return resultado;
}


void setup() 
{
  inputString = ""; 
  stringComplete = false;
  
  //rele
  pinMode(2, OUTPUT);  
  pinMode(3, OUTPUT);  
  pinMode(4, OUTPUT);  
  pinMode(5, OUTPUT);  
  
  //motor passo
  pinMode(6, OUTPUT);  
  pinMode(7, OUTPUT);  
  pinMode(8, OUTPUT);  
  pinMode(9, OUTPUT);  
  pinMode(sensorp, INPUT); 
  pinMode(piscaled, OUTPUT);   
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  digitalWrite(2, LOW); 
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
  //Motor de passo
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  StatusSD(0);
  
  

  
}


bool MountSD(int Tipo)
{
 //Configurando cartao SD
 SetupSD(Tipo);

 // print the type of card
  Println(Tipo,"\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Println(Tipo,"SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Println(Tipo,"SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Println(Tipo,"SDHC");
      break;
    default:
      Println(Tipo,"Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) 
  {
    Println(Tipo,"Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return false;
  } 
  else
  { 

    Println(0,"\nVolume type is FAT");
    Serial.println(volume.fatType(), DEC);
    
    
    StatusSD(Tipo);
    return true;
  }
}



//Mostra o status do SD
void StatusSD(int Tipo)
{
    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    volumesize *= 512;                            // SD card blocks are always 512 bytes
    Println(Tipo,"Volume size (bytes): ");
    Serial.println(volumesize);
    Println(Tipo,"Volume size (Kbytes): ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.println("Volume size (Mbytes): ");
    volumesize /= 1024;
    Serial.println(volumesize);
  
    
    Println(Tipo,"\nArquivos encontrados no SD (name, date and size in bytes): ");
    //root.openRoot(volume);
    //root.ls(LS_R | LS_DATE | LS_SIZE);
  
}

void Status(int Tipo)
{
  Println(Tipo,"Status da aplicação"); 
  StatusSD(Tipo);
}
 

//Retorna na serial todos os arquivos
void lsSd(int Tipo)
{
  root.openRoot(volume);
  //list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);  
  
}

//Analisa os comandos
bool AnalisaCMD(String info, int Tipo)
{
  
  bool processo = false;
  if(info == "help")
  {
    MostraHelp(Tipo);
    processo = true;
  }
  if(info == "status")
  {
    Status(Tipo);
    processo = true;
  }
  
  if(info == "11")
  {
      digitalWrite(2, HIGH); 
      Println(Tipo,"Levantou Rele");
      processo = true;
  }
  if(info == "10")
  {
      digitalWrite(2, LOW); 
      Println(Tipo,"Baixou Rele");  
      processo = true;
  }
  if(info == "21")
  {
      digitalWrite(3, HIGH); 
      Println(Tipo,"Levantou Rele");
      processo = true;
  }
  if(info == "20")
  {
      digitalWrite(3, LOW); 
      Println(Tipo,"Baixou Rele");  
      processo = true;
  }
  
  if(info == "30")
  {
      digitalWrite(4, LOW); 
      Println(Tipo,"Baixou Rele");  
      processo = true;
  }
  
  if(info == "31")
  {
      digitalWrite(4, HIGH); 
      Println(Tipo,"Levantou Rele");
      processo = true;
  }

  if(info == "41")
  {    
      digitalWrite(5, HIGH); 
      Println(Tipo,"Levantou Rele");
      processo = true;
  }
  if(info == "40")
  {
      digitalWrite(5, LOW); 
      Println(Tipo,"Baixou Rele");  
      processo = true;
  }
  //Mount SD
  if(info == "mount")
  {  
     //MountSD(Tipo);
     processo = true;
  }
  
  
  //Mount SD
  if(info == "ls")
  {  
     lsSd(Tipo);
     processo = true;
  }
  
  //Mount SD
  if(info == "pisca")
  {  
     delay(500);
     digitalWrite(piscaled, HIGH);
     delay(500);
     digitalWrite(piscaled,LOW);
     Println(Tipo,"Piscou led!");
  }
  return processo;
  
}


void MostraHelp(int Tipo)
{
 Println(Tipo,"Sistema de Controle residencial!"); 
 Println(Tipo,"Versao "+strversao);
 Println(Tipo,"Help - Comandos válidos");
 Println(Tipo,"1x - Rele 1");
 Println(Tipo,"2x - Rele 2");
 Println(Tipo,"3x - Rele 3");
 Println(Tipo,"4x - Rele 4");
 
 Println(Tipo,"M1x - Servo 1");
 Println(Tipo,"M2x - Servo 2");
 Println(Tipo,"M3x - Servo 3");
 Println(Tipo,"M4x - Servo 4");
 
 Println(Tipo,"help - Mostra este menu");
 Println(Tipo,"status - situação do equipamento");  
 Println(Tipo,"ls - diretorio do SD");  
 Println(Tipo,"mount - monta volume do SD");  
}


//sensor de presença
bool LeSensorP()
{
 info = 0;
 info = digitalRead(sensorp);
 
 return (info!=0);      //Faz leiturado do Botão e joga o valor na variálvel "Bot"   
}

void loop() 
{
  
  //se tem alguem acende luz 1]
  /*
  if (LeSensorP()== true)
  {
    Serial.println("Registrou movimento!"); 
    AnalisaCMD("pisca");

  }
  */
  // print the string when a newline arrives:
  if (stringComplete) {
    Println(0,"Um comando foi recebido:"+ inputString);
    if (!(inputString == "")) 
    {
      if( AnalisaCMD(inputString,0)== true)
      {
         Println(0,"Comando processado"); 
      }
      else
      {
        Println(0,"Comando inválido!");
      }
      inputString = "";
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
      stringComplete = true;
    }
     else
    { 
      // add it to the inputString:
      inputString += inChar;
    } 
  }
}


