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


 The circuit:
 =================================
 LCD pin              Connect to
 ---------------------------------
 01 - GND             GND, pot
 02 - VCC             +5V, pot
 03 - Contrast        Pot wiper
 04 - RS              Pin8 (P1.0)
 05 - R/W             GND
 06 - EN              Pin9 (P1.1)
 07 - DB0             GND
 08 - DB1             GND
 09 - DB2             GND
 10 - DB3             GND
 11 - DB4             Pin10 (P1.2)
 12 - DB5             Pin11 (P1.3)
 13 - DB6             Pin12 (P1.4)
 14 - DB7             Pin13 (P2.3)
 15 - BL+             +5V
 16 - BL-             GND
 =================================

  
*/

// CÃ³digo teste modificado com base no escrito pelo autor acima.
#include "Mfrc522.h"
#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(P1_0, P1_1, P1_2, P1_3, P1_4, P2_3);

int CS = 8;                                 // chip select pin
int NRSTDP = 5;
//int rele = 9;             //pino do rele in pino 9
Mfrc522 Mfrc522(CS, NRSTDP);
unsigned char serNum[5];


void setup()
{
  Serial.begin(9600);
  Serial.println("Iniciando RFID\n");
      
  SPI.begin();
  digitalWrite(CS, LOW);                    // Initialize the card reader
  //pinMode(rele, OUTPUT);
  //digitalWrite(rele, LOW);
  pinMode(RED_LED, OUTPUT);                 // Blink LED if card detected
  Mfrc522.Init();
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Iniciando RFID\n");
  //lcd.print("Passe o cartão!");
  //lcd.clear();
}

void loop()
{
  unsigned char status;
  unsigned char str[MAX_LEN];

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
       Serial.println("Acesso do bilhete unico autorizado\n");
       lcd.clear();
       lcd.setCursor(16, 2);
       lcd.print("Bem Vindo Felicio !");
       digitalWrite(RED_LED, HIGH);
       //delay(3000);              // espera 3 segundos
       //digitalWrite(rele, LOW);  // desativa rele, fecha a trava solenoide    
       }
     
    else
        {
      Serial.println("Acesso negado!!!\n");
      lcd.clear();    
      lcd.print("Acesso negado!");    
       }
    
    delay(1000);
    digitalWrite(RED_LED, LOW);
  }
  Mfrc522.Halt();
}

