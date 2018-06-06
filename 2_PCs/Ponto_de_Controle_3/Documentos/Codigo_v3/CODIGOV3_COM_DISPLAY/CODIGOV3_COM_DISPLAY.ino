
/*
  Example for TI MSP430 LaunchPads and Energia that reads a card number
  using a RC522 MIFARE module, takes action depending on the card number,
  and prints it to the serial monitor.
  https://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm
  Based on code and ideas from Eelco Rouw (www.43oh.com), Grant Gibson
  (www.grantgibson.co.uk), Dr.Leong (www.b2cqshop.com), and
  Craig Thompson/Aaron Norris at Addicore.
  Minor modifications to above by Frank Milburn 10 June 2015
  Released into the public domain
  Tested on MSP-EXP430G2 LaunchPad
          MSP-EXP430F5529 LaunchPad
          MSP-EXP430FR5969 LaunchPad
  Pin Connections
  ===================================
  RFID Module       MSP430 LaunchPads
  --------------    -----------------
  Pin 1  (SDA)      Pin 8  (CS)
  Pin 2  (SCK)      Pin 7  (SCK)
  Pin 3  (MOSI)     Pin 15 (MOSI)
  Pin 4  (MISO)     Pin 14 (MISO)
  Pin 5  (IRQ)      Not connected
  Pin 6  (GND)      GND
  Pin 7  (RST)      Pin 10
  Pin 8  (3V3)      3V3
  Addicore has a very good introduction to this module, written for Arduino.
  Try the site below for additional detail on the module and examples
  which include writing to a card, dumping detailed information, changing
  the card user ID, etc.  It will run on LaunchPads or Arduinos with the
  correct pin connections:  https://github.com/miguelbalboa/rfid
*/

// CÃ³digo teste modificado com base no escrito pelo autor acima.
#include "Mfrc522.h"
#include "LiquidCrystal.h"
#include <SPI.h>

LiquidCrystal lcd(P2_6, P2_7, P1_3, P1_4, P2_3, P2_4);
//LiquidCrystal lcd(3, 4, 5, 6, 11, 12);

int CS = 8;                                 // chip select pin
int NRSTDP = 9;
int rele = P1_0;             //pino do rele in pino 2
Mfrc522 Mfrc522(CS, NRSTDP);
unsigned char serNum[5];

void setup()
{
  lcd.begin(16,2);
  lcd.print("RFID LOADING...");
  delay(5000);
  lcd.clear();
  
  
  Serial.begin(9600);
  //Serial.println("Iniciando RFID\n");
  
  SPI.begin(); 
  digitalWrite(CS, LOW);                    // Initialize the card reader
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);
  pinMode(RED_LED, OUTPUT);                 // Blink LED if card detected
  Mfrc522.Init();

 }

void loop()
{
  void home(); 
  unsigned char status;
  unsigned char str[MAX_LEN];

  lcd.print("PASSE O CARTAO");
  delay(250); // 1/4 de segundos
  lcd.clear();
  lcd.print("PASSE O CARTAO.");
  lcd.clear();
  delay(250); // 1/4 de segundos
  lcd.print("PASSE O CARTAO..");
  delay(250); // 1/4 segundos
  lcd.clear();
     
  status = Mfrc522.Request(PICC_REQIDL, str);
    if (status == MI_OK)
  {
    
    Serial.print("RFID detectado: ");
    Serial.print(str[0], BIN);
    Serial.print(" , ");
    Serial.print(str[1], BIN);
    Serial.println("");
  }
   status = Mfrc522.Anticoll(str);
   memcpy(serNum, str, 5);
  if (status == MI_OK)
  {
    digitalWrite(RED_LED, LOW);              // Card or tag detected!
    Serial.print("ID: ");
    Serial.print(serNum[0], HEX);
    Serial.print(" , ");
    Serial.print(serNum[1], HEX);
    Serial.print(" , ");
    Serial.print(serNum[2], HEX);
    Serial.print(" , ");
    Serial.print(serNum[3], HEX);
    Serial.print(" , ");
    Serial.print(serNum[4], HEX);
    Serial.println(""); 

    
    // Additional cards can be recognized by running the program and noting the 5 card specific numbers
    // and then adding an "else if" statement below.
    if (serNum[0] == 0xBB && serNum[1] == 0x01 && serNum[2] == 0xB5 && serNum[3] == 0xA9 && serNum[4] == 0xA6)
       {
       lcd.setCursor( 3 , 0 );
       lcd.print("BEM VINDO,"); // printar a msg de acesso 
       lcd.setCursor( 4 , 1 ); //ajustando a posição
       lcd.print("SR. FELICIO !"); // printar na parte de baixo do display
       digitalWrite(RED_LED, HIGH); //Acender o led da placa como sinal
       digitalWrite(rele, HIGH); //Aticar o relê
       delay(3000);              // espera 3 segundos
       digitalWrite(rele, LOW);  // desativa rele, fecha a trava solenoide
       digitalWrite(RED_LED, LOW);

       }
       
      else if (serNum[0] == 253 && serNum[1] == 125 && serNum[2] == 181 && serNum[3] == 21 && serNum[4] == 32)
       {
       lcd.setCursor( 3 , 0 );
       lcd.print("BEM VINDO,"); // printar a msg de acesso 
       lcd.setCursor( 4 , 1 ); //ajustando a posição
       lcd.print("SR. BAPTISTA!"); // printar na parte de baixo do display
       digitalWrite(RED_LED, HIGH); //Acender o led da placa como sinal
       digitalWrite(rele, HIGH); //Aticar o relê
       delay(3000);              // espera 3 segundos
       digitalWrite(rele, LOW);  // desativa rele, fecha a trava solenoide
       digitalWrite(RED_LED, LOW);

        
       }
     
    else
        {
       lcd.setCursor(0, 0); // ajustando a posição   
       lcd.print("ACESSO NEGADO..."); //Printar a msg quando o acesso for negado 
       lcd.setCursor(0 , 1); // ajustando a posição de baixo
       lcd.print("TENTE NOVAMENTE");
       delay(3000);  //3s 
           
        }
    
   
  }
  
  Mfrc522.Halt();
  
}

