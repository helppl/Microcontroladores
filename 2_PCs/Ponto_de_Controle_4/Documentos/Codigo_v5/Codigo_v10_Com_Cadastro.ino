/*
   AUTOR: JOÃO VITOR RODRIGUES BAPTISTA
   REPOSITORIO: https://github.com/helpthx
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
#include <msp430g2553.h>
#include "Mfrc522.h"
#include "LiquidCrystal.h"
#include <SPI.h>


LiquidCrystal lcd(P2_6, P2_7, P1_3, P1_4, P2_3, P2_4);

int CS = 8;  // pino 8
int NRSTDP = 9; //pino 9
int rele = 2;  //pino do rele in pino 2
Mfrc522 Mfrc522(CS, NRSTDP);
unsigned char serNum[5];
int ENTRADA1[10] = {0};

int ENTRADA;
char nome_cliente[16];
unsigned char reg[10];
unsigned char reg1[10];
unsigned char reg2[10];
unsigned char reg3[10];
unsigned char reg4[10];
int inByte = 0; // incoming serial byte
int inByteS = 0;
int i = 0;

//Inicia todos os componentes display, frid e comnucação serial com o Energia
void setup()
{
  lcd.begin(16, 2);
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
          Interface inicial com o usuario, "PASSE O CARTÃO" piscando de 1/2s e 1/2s
       ******************************************************************************* */
  lcd.print("PASSE O CARTAO");
  delay(500); // 1/2 de segundos
  lcd.clear();
  lcd.print("PASSE O CARTAO.");
  delay(500); // 1/2 de segundos
  lcd.clear();
  lcd.print("PASSE O CARTAO..");
  delay(500); // 1/2 segundos
  lcd.clear();
  //Serial.print("\nPASSE O CARTAO");
  // delay(1000);
  /* *******************************************************************************
        Interface serial entre o MSP430 e o Energia para diferenciar os cartões  !
     ******************************************************************************* */

  if (Serial.available() > 0) {
    // get incoming byte:
    inByteS = Serial.read();
  }
  if (inByteS == 49) { // se for 1 ele vai para o modo de controlar a catraca
    comandos_pc();
    inByteS = 0;
  }
  else if(inByteS == 50){ // se for 2 ele vai para o modo de cadastro

    delay(2000);
    int j = 1; 
    while(j){
    delay(2000);
    Serial.print("PASSE O CARTÃO QUE VAI SER CADASTRADO\n");
    
    status = Mfrc522.Request(PICC_REQIDL, str);
    if (status == MI_OK)
  {

    Serial.print("Dedectando RFID...\n");
   // Serial.print(str[0], BIN);
   // Serial.print(" , ");
   // Serial.print(str[1], BIN);
   // Serial.println("");
  }
   delay(1000);
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
     reg[i] = serNum[0];
     reg1[i] = serNum[1];
     reg2[i] = serNum[2];
     reg3[i] = serNum[3];
     reg4[i] = serNum[4];
    Serial.print("DADOS SALVO NO REGISTRADOR: ");
    Serial.print(i);
    Serial.print(" , ");
    Serial.print(reg[i], HEX);
    Serial.print(" , ");
    Serial.print(reg1[i], HEX);
    Serial.print(" , ");
    Serial.print(reg2[i], HEX);
    Serial.print(" , ");
    Serial.print(reg3[i], HEX);
    Serial.print(" , ");
    Serial.print(reg4[i], HEX);
    Serial.println("");
    Serial.print("SALVO COM SUCESSO!!!\n");
    i++;
    inByteS = 0;
    j = 0;
    }
    break;
    } 
    
  }
  else {
    status = Mfrc522.Request(PICC_REQIDL, str);
    if (status == MI_OK)
    {

      Serial.print("\nRFID detectado:");
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
          Parte feita para verificar as condições: se o cartão esta ou não cadastrado !
       ********************************************************************************** */


   int SinalR  = 0;
   int contador; //variável de controle do loop
   for(contador = 0; contador < 10; contador++){
       if (serNum[0] == reg[contador] && serNum[1] == reg1[contador] && serNum[2] == reg2[contador] && serNum[3] == reg3[contador] && serNum[4] == reg4[contador])
       {
     
       ENTRADA1[contador] = FuncPrintar(ENTRADA1[contador], contador);
       
       SinalR = 1;
       }
      }

    if(SinalR == 0){
      Serial.print("TENTE NOVAMENTE\n");
      }

    }
    Mfrc522.Halt();
  }
}


//FUNÇÃO GERARAL PARA PRINTAR
int FuncPrintar(int ENTRADA, int contador) {
  //ENTRADA GENERICA

  if (ENTRADA == 0) {
    lcd.setCursor( 3 , 0 );
    lcd.print("BEM VINDO,"); // printar a msg de acesso
    lcd.setCursor( 4 , 1 ); //ajustando a posição
    lcd.print(nome_cliente); // printar na parte de baixo do display
    ENTRADA = 1;
    Entrada_Serial(contador);
    AbrirPorta();
    delay(2000);
    FecharPorta();
  }

  else if (ENTRADA == 1) {
    lcd.setCursor( 2 , 0 );
    lcd.print("VOLTE SEMPRE,"); // printar a msg de acesso
    lcd.setCursor( 4 , 1 ); //ajustando a posição
    lcd.print(nome_cliente);
    ENTRADA = 0;
    Saida_Serial(contador);
    AbrirPorta();
    delay(2000);
    FecharPorta();
  }

  else if (ENTRADA == 2) {
    Tente_Serial();
    lcd.setCursor(0, 0); // ajustando a posição
    lcd.print("ACESSO NEGADO..."); //Printar a msg quando o acesso for negado
    lcd.setCursor(0 , 1); // ajustando a posição de baixo
    lcd.print("TENTE NOVAMENTE");
    delay(2000);  //5s
  }
  else {
    lcd.print("ERROR 3#");
    delay(5000);
  }

  return ENTRADA;

}


//ABRIR O RELÊ
int AbrirPorta(void) {
  digitalWrite(RED_LED, HIGH); //Acender o led da placa como sinal
  digitalWrite(rele, HIGH); //Aticar o relê
  return 0;
}

// FECHAR O RELÊ
int FecharPorta(void) {
  digitalWrite(rele, LOW);  // desativa rele, fecha a trava solenoide
  digitalWrite(RED_LED, LOW);
  return 0;
}



/* ********************************************************************************
       Funções para transmissão serial dos dados para serem salvos numa pasta   !
    ********************************************************************************** */

int Entrada_Serial(int contador) {
  Serial.print("BEM VINDO, CADASTRO: "); // printar a msg de acesso
  Serial.print(contador); // printar na parte de baixo do display

  return 0;
}


int Saida_Serial(int contador) {
  Serial.print("VOLTE SEMPRE,CADASTRO: "); // printar a msg de acesso
  Serial.print(contador); // printar na parte de baixo do display

  return 0;
}

int Tente_Serial(void) {
  Serial.print("ACESSO NEGADO...\n"); // printar a msg de acesso
  Serial.print("TENTE NOVAMENTE\n"); // printar na parte de baixo do display
  return 0;
}

/* ********************************************************************************
      COMANDOS DE ENTRADA E SAIDA PELO PC                                       !
   ********************************************************************************** */


int comandos_pc(void) {

  int i = 1;
  inByte = 0;
  while (i) {


    if (Serial.available() > 0) {
      // get incoming byte:

      inByte = Serial.read();
    }

    if (inByte == 65 ) { // A ou a ou 1
      Serial.print("ABERTO\n");
      AbrirPorta();
      delay(1000);
    }
    else if (inByte == 70 ) { //F ou f 70 102
      Serial.print("FECHADO\n");
      FecharPorta();
      delay(1000);
    }
    else if (inByte == 83 ) { //S ou f 83 115
      Serial.print("VOLTAR PARA O PADRAO\n");
      inByte = 0;
      i = 0;
      delay(1000);
    }
    else {
      Serial.print("\nEscolha A para abrir ou F para fechar ou S para voltar p menu inicial\n");
      delay(2000);
    }
  }
}



/*
  #include <msp430g2553.h>
  #include "Mfrc522.h"
  #include "LiquidCrystal.h"
  #include <SPI.h>

  int CS = 8;  // pino 8
  int NRSTDP = 9; //pino 9
  int rele = 2;  //pino do rele in pino 2
  Mfrc522 Mfrc522(CS, NRSTDP);
  unsigned char serNum[5];
  int ENTRADA1 = 0;
  int ENTRADA2 = 0;
  int ENTRADA;
  char nome_cliente[16];
  unsigned char reg[10];
  unsigned char reg1[10];
  unsigned char reg2[10];
  unsigned char reg3[10];
  unsigned char reg4[10];
  int inByte = 0; // incoming serial byte
  int inByteS = 0;
  int i = 0;

  void setup()
  {

  Serial.begin(9600);
  Serial.println("Iniciando RFID\n");

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



  /*   *******************************************************************************
        Interface inicial com o usuario, "PASSE O CARTÃO" piscando de 1/2s e 1/2s
     *******************************************************************************
  Serial.print("\nPASSE O CARTAO");
  delay(1000);
  /* *******************************************************************************
        Interface serial entre o MSP430 e o Energia para diferenciar os cartões  !
     *******************************************************************************

   if (Serial.available() > 0) {
    // get incoming byte:
    inByteS = Serial.read();
      }
  if(inByteS == 49){ // se for 1 ele vai para o modo de controlar a catraca
    Serial.print("\nMODO DE CONTROLE PC");
    inByteS = 0;
  }
  else if(inByteS == 50){ // se for 2 ele vai para o modo de cadastro

    delay(2000);
    while(1){

    delay(3000);
    Serial.print("\nMODO CADASTRO");
    status = Mfrc522.Request(PICC_REQIDL, str);
    if (status == MI_OK)
  {

    Serial.print("\nRFID detectado:");
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
     delay(1000);

     reg[i] = serNum[0];
     reg1[i] = serNum[1];
     reg2[i] = serNum[2];
     reg3[i] = serNum[3];
     reg4[i] = serNum[4];
    Serial.print("RESULTADOS DA TRANSFERENCIA DE REG 0\n");
    Serial.print(i);
    Serial.print(" , ");
    Serial.print("ID: ");
    Serial.print(reg[0], HEX);
    Serial.print(" , ");
    Serial.print(reg1[0], HEX);
    Serial.print(" , ");
    Serial.print(reg2[0], HEX);
    Serial.print(" , ");
    Serial.print(reg3[0], HEX);
    Serial.print(" , ");
    Serial.print(reg4[0], HEX);
    Serial.println("");
    Serial.print("RESULTADOS DA TRANSFERENCIA DE REG 1\n");
    Serial.print(i);
    Serial.print(" , ");
    Serial.print(reg[1], HEX);
    Serial.print(" , ");
    Serial.print(reg1[1], HEX);
    Serial.print(" , ");
    Serial.print(reg2[1], HEX);
    Serial.print(" , ");
    Serial.print(reg3[1], HEX);
    Serial.print(" , ");
    Serial.print(reg4[1], HEX);
    Serial.println("");
    Serial.print("RESULTADOS DA TRANSFERENCIA DE REG 1\n");
    Serial.print(i);
    Serial.print(" , ");
    Serial.print(reg[2], HEX);
    Serial.print(" , ");
    Serial.print(reg1[2], HEX);
    Serial.print(" , ");
    Serial.print(reg2[2], HEX);
    Serial.print(" , ");
    Serial.print(reg3[2], HEX);
    Serial.print(" , ");
    Serial.print(reg4[2], HEX);
    Serial.println("");
    Serial.print("RESULTADOS DA TRANSFERENCIA DE REG 1\n");
    Serial.print(i);
    Serial.print(" , ");
    Serial.print(reg[i], HEX);
    Serial.print(" , ");
    Serial.print(reg1[i], HEX);
    Serial.print(" , ");
    Serial.print(reg2[i], HEX);
    Serial.print(" , ");
    Serial.print(reg3[i], HEX);
    Serial.print(" , ");
    Serial.print(reg4[i], HEX);
    Serial.println("");
    break;
    }

    i++;;
    }
  }
  else{
   status = Mfrc522.Request(PICC_REQIDL, str);
    if (status == MI_OK)
  {

    Serial.print("\nRFID detectado:");
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
        Parte feita para verificar as condições: se o cartão esta ou não cadastrado !
     **********************************************************************************


    if(serNum[0] == 0xBB && serNum[1] == 0x01 && serNum[2] == 0xB5 && serNum[3] == 0xA9 && serNum[4] == 0xA6) //SR.FELICIO
    {
       Serial.print("\nJOAO");

      }
    else if (serNum[0] == 253 && serNum[1] == 125 && serNum[2] == 181 && serNum[3] == 21 && serNum[4] == 32) //SR. BAPISTA
       {
       Serial.print("\nFELICIO");
       }
    else
       {
       Serial.print("\nTENTE NOVAMENTE");
       }

  }
  Mfrc522.Halt();
  }
  }




*/
