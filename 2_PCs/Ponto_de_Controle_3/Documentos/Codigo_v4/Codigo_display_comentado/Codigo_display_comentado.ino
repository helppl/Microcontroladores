
/*
 * AUTOR: JOÃO VITOR RODRIGUES BAPTISTA
 * REPOSITORIO: https://github.com/helpthx
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

  Pin Connections
  ===================================
  Display Module    MSP430 LaunchPads
  --------------    -----------------
  EN                P2_6     
  RS                P2_7
  D4                P1_3
  D5                P1_4    
  D6                P2_3
  D7                P2_4  

 Conexões de ground e vcc no proprio hardware...

 Código: ler um valor de um cartão: Autoriza os cartões cadastrados abrindo o rele,
 quando o cartão não esta cadastrado ele volta no loop.
*/

#include "Mfrc522.h"
#include "LiquidCrystal.h"
#include <SPI.h>

LiquidCrystal lcd(P2_6, P2_7, P1_3, P1_4, P2_3, P2_4);

int CS = 8;                                 // chip select pin
int NRSTDP = 9;
int rele = P1_0;                            //pino do rele in pino 2
Mfrc522 Mfrc522(CS, NRSTDP);
unsigned char serNum[5];

//Inicia todos os componentes display, frid e comnucação serial com o Energia
void setup() 
{
  lcd.begin(16,2);
  lcd.print("RFID LOADING...");
  delay(3000);
  lcd.clear();
    
  Serial.begin(9600);
  Serial.println("Iniciando RFID\n");
  
  SPI.begin(); 
  digitalWrite(CS, LOW);                    // Initialize the card reader
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);
  pinMode(RED_LED, OUTPUT);                 // Blink LED if card detected
  Mfrc522.Init();

 }
// Loop principal do sistema
void loop()
{
  void home(); 
  unsigned char status;
  unsigned char str[MAX_LEN];

/*   *******************************************************************************
     *  Interface inicial com o usuario, "PASSE O CARTÃO" piscando de 1/2s e 1/2s  * 
     ******************************************************************************* */
  lcd.print("PASSE O CARTAO");
  delay(500) // 1/2 de segundos
  lcd.clear();
  lcd.print("PASSE O CARTAO.");
  lcd.clear();
  delay(500); // 1/2 de segundos
  lcd.print("PASSE O CARTAO..");
  delay(500); // 1/2 segundos
  lcd.clear();

  /* *******************************************************************************
     *  Interface serial entre o MSP430 e o Energia para diferenciar os cartões  ! * 
     ******************************************************************************* */
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

    /* ********************************************************************************
     *  Parte feita para verificar as condições: se o cartão esta ou não cadastrado ! * 
     ********************************************************************************** */
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

