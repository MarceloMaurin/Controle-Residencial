/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
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
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  digitalWrite(2, LOW); 
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

//Analisa os comandos
void AnalisaCMD(String info)
{
  if(info == "11")
  {
      digitalWrite(2, HIGH); 
      Serial.println("Levantou Rele");
  }
  if(info == "10")
  {
      digitalWrite(2, LOW); 
      Serial.println("Baixou Rele");  
  }
  if(info == "21")
  {
      digitalWrite(3, HIGH); 
      Serial.println("Levantou Rele");
  }
  if(info == "20")
  {
      digitalWrite(3, LOW); 
      Serial.println("Baixou Rele");  
  }
  
  if(info == "31")
  {
      digitalWrite(4, HIGH); 
      Serial.println("Levantou Rele");
  }
  if(info == "30")
  {
      digitalWrite(4, LOW); 
      Serial.println("Baixou Rele");  
  }
  if(info == "41")
  {
    
      digitalWrite(5, HIGH); 
      Serial.println("Levantou Rele");
  }
  if(info == "40")
  {
      digitalWrite(5, LOW); 
      Serial.println("Baixou Rele");  
  }

  
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println("Foi lido um byte");
    Serial.println(inputString); 
    if (!(inputString == "")) 
    {
      AnalisaCMD(inputString); 
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


