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

   ===================================
  Rele                MSP430 LaunchPads
  --------------    -----------------
  Pin in 3          Pin 2 (LED1)

  Código: ler um valor de um cartão: Autoriza os cartões cadastrados abrindo o rele,
  Cadastra novos cartões, Descadastra cartões e ainda tem comando remodo via serial.
*/

#include <msp430g2533.h>
#include <Serial_UART.h>
#include <Serial_SPI.h>
#include <Rc522.h>
#include <string.h>

#define TEST BIT0
#define CS BIT0
#define NRSTDP BIT3
Rc522 rc522(CS, NRSTDP);
unsigned char serNum[5];
unsigned char reg[10];
unsigned char reg1[10];
unsigned char reg2[10];
unsigned char reg3[10];
unsigned char reg4[10];



int i = 1;
int l = 0;

int ENTRADA[10] = {0};
int inByteD = 0;


// ABRIR O RELÊ
 void Abrir_Porta(){
     while(1){
     digitalWriteA(1, TEST, HIGH); //Ativar o rele
     break;
     }
 }

// FECHAR O RELÊ
 void Fechar_Porta(){
     while(1){
     digitalWriteA(1, TEST, LOW); //Desativar o rele
     break;
     }
    }


 /*  *******************************************************************************
     *              Função para o modo de comando abrir e fechar pelo pc         ! *
     ******************************************************************************* */
void comandos_pc() {

  int h = 1;
  int inByte = 0;
  while (h) {
    if(Serial_UART.available() != 0) {
        Serial_UART.readInt();
        Serial_UART.print(inByte);
        Serial_UART.print("\n");
            if (inByte == 65 ) { // A
                    Serial_UART.print("\nABERTO\n");
                    Abrir_Porta();
                    delayms(1000);
            }
            else if (inByte == 70 ) { //F
                    Serial_UART.print("\nFECHADO\n");
                    Fechar_Porta();
                    delayms(1000);
            }
            else if (inByte == 83 ) { //S
                    Serial_UART.print("\nVOLTAR PARA MODO DE LEITURA\n");
                    inByte = 0;
                    h = 0;
                    delayms(1000);
            }
            else { //Erro 1: Comando não cadastrado.
                    Serial_UART.print("ERRO 1:\n");
                    delayms(2000);
            }
    }
    else {

        Serial_UART.print("\nPara Abrir(A), Fechar(F) ou Modo de Leitura(S)");

        delayms(2000);
   }
  }
}

/*  *******************************************************************************
    *              Função para o modo de apagar registrador de cadastro         ! *
    ******************************************************************************* */

void apagador_reg() {
  int x = 1;
  int inByteD = 0;
 while (x) {
      if(Serial_UART.available() != 0) {
          Serial_UART.readInt();
          Serial_UART.print(inByteD);
                 if (inByteD != 0 ) { //
                  l = (inByteD - 48);
                  reg[l] = 00;
                  reg1[l] = 00;
                  reg2[l] = 00;
                  reg3[l] = 00;
                  reg4[l] = 00;
                  Serial_UART.print("\nDADOS APAGADOS DO REGISTRADOR: ");
                  Serial_UART.print(l);
                  Serial_UART.print("\n");
                  Serial_UART.print(reg[l]);
                  Serial_UART.print(" , ");
                  Serial_UART.print(reg1[l]);
                  Serial_UART.print(" , ");
                  Serial_UART.print(reg2[l]);
                  Serial_UART.print(" , ");
                  Serial_UART.print(reg3[l]);
                  Serial_UART.print(" , ");
                  Serial_UART.print(reg4[l]);
                  Serial_UART.print(".");
                  Serial_UART.print("\nDADOS APAGADOS COM SUCESSO !!!");
                  i = l;
                  x = 0;
                  delayms(2000);
              }

              else{ //Erro 2: Numero do registrador invalido.
                  Serial_UART.print("\nERRO 2:");
              }
      }
    else {
        Serial_UART.print("DIGITE O NUMERO DO REGISTRADO QUE VOCE DESEJA APAGAR!\n");
        delayms(2000);
        }
  }
}

/*  *******************************************************************************
    *               Função para setar o registrador na posição X !                *
    ******************************************************************************* */
/*
int setar_reg(volatile int k) {
  int y = 1;
  int inByteKK = 0;
  while (y) {
      if(Serial_UART.available() != 0) {
         Serial_UART.readInt();
         Serial_UART.print(inByteKK);
              if (inByteKK != 0 ) { //
                  k = (inByteKK - 48);
                  Serial_UART.print("POSICAO ATUAL: \n");
                  Serial_UART.print(k);
                  y = 0;

              }
              else{ //Erro 2: Numero do registrador invalido.
                  Serial_UART.print("\nERRO 2:");
              }
      }
    else {
        Serial_UART.print("DIGITE A POSICAO QUE VOCE QUER COLOCAR O REGISTRADOR !\n");
        delayms(2000);
        }
  }
 return (k);
} */



 /*  *******************************************************************************
     *  Funções para transmissão serial dos dados para serem salvos numa pasta   ! *
     ******************************************************************************* */

 void Entrada_Serial(volatile int contador)
     {
     while(1){
     Serial_UART.print("ENTRADA DO CADASTRO: ");
     Serial_UART.print(contador);
     Serial_UART.print("\n");
     break;
     }
 }


 void Saida_Serial(volatile int contador){
     while(1){
      Serial_UART.print("SAIDA DO CADASTRO: ");
      Serial_UART.print(contador);
      Serial_UART.print("\n");
      break;
     }


 }

 void Tente_Serial(){
     while(1){
     Serial_UART.print("ACESSO NEGADO...");
     //Serial_UART.print("TENTE NOVAMENTE\n");
     Serial_UART.print("\n");
     break;
     }

}

 void Tela_Espera(){
      while(1){
      Serial_UART.print("PASSE O CARTAO\n");
      Serial_UART.print("\n");
      break;
      }

 }

 /*  *******************************************************************************
      *        Função geral entrada/saida e chamar as funções de print Serial    ! *
      ****************************************************************************** */
int Func_Printar(volatile int ENTRADA, volatile int contador){

    if(ENTRADA == 0){
          Entrada_Serial(contador);
          Abrir_Porta();
          delayms(2000);
          Fechar_Porta();

          ENTRADA = 1;
         }

     else if (ENTRADA == 1){
            Saida_Serial(contador);
            Abrir_Porta();
            delayms(2000);
            Fechar_Porta();

            ENTRADA = 0;

          }

      else if (ENTRADA == 2) {
          Tente_Serial();

           }
      else { //Erro 3: Parametro de entrada e saida sem ser 0(Entrada) ou 1(Saida)
          Serial_UART.print("ERRO 3");
           }

    return (ENTRADA);

 }

/*   ******************************************************************************
     *                        Função main do projeto !                            *
     ****************************************************************************** */
int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT


    Serial_UART.init(9600);
    Serial_UART.print("\nRFID LOADING");
    Serial_UART.print("\r\n");
    int a = 0;
    reg[0]=253;
    reg1[0]=125;
    reg2[0]=181;
    reg3[0]=21;
    reg4[0]=32;
  //  digitalWriteA(1, TEST, LOW);
  //  Serial_SPI.init();
   // digitalWriteA(1, CS, LOW);
   // pinModeA(1, TEST, OUTPUT);
   // rc522.Init();


  while (1){
          WDTCTL = WDTPW + WDTHOLD; // Stop WDT
          Serial_UART.init(9600);
          digitalWriteA(1, TEST, LOW);
          Serial_SPI.init();
          digitalWriteA(1, CS, LOW);
          pinModeA(1, TEST, OUTPUT);
          rc522.Init();



        unsigned char status;
        unsigned char str[MAX_LEN];

        delayms(1000);
       if(Serial_UART.available() != 0){
           Serial_UART.readInt();
           Serial_UART.print(a);
              if(a == 49){ //CHAMA MODO DE COMANDO NO PC
                      Serial_UART.print("\nMODO DE COMANDO PELO PC\n");
                      comandos_pc();
                  }
               else if(a == 51){ //CHAMA O MODO DE APAGAR CADASTRO
                       Serial_UART.print("\nMODO DE APAGAR O CADASTRO\n");
                       apagador_reg();

               }
               else if(a == 50)
               { //CHAMA MODO DE CADASTRO
                  Serial_UART.print("\nMODO DE CADASTRO\n");
                  delayms(2000);

                  int j = 1;
                  while(j)
                 {
                 delayms(2000);
                 Serial_UART.print("\nPASSE O CARTAO QUE VAI SER CADASTRADO OU PRESSIONE (S)\n");

                 if(Serial_UART.available() != 0) { //Codigo para sair precionando S
                        Serial_UART.readInt();
                        Serial_UART.print(a);
                                 if (a == 83 ) { // S
                                    Serial_UART.print("\nSAINDO PARA MODO DE LEITURA\n");
                                    j = 0;

                            }}

                 status = rc522.Request(PICC_REQIDL, str);
                 if (status == MI_OK)
                 {
                  Serial_UART.print("DETECTANDO CARTÃO...\n");
                 delayms(1000);

                 status = rc522.Anticoll(str);
                 memcpy(serNum, str, 5);
                 if (status == MI_OK)
                 {

                 Serial_UART.print("ID: ");
                 Serial_UART.print(serNum[0]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(serNum[1]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(serNum[2]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(serNum[3]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(serNum[4]);
                 Serial_UART.print(".");
                 reg[i] = serNum[0];
                 reg1[i] = serNum[1];
                 reg2[i] = serNum[2];
                 reg3[i] = serNum[3];
                 reg4[i] = serNum[4];
                 Serial_UART.print("\nDADOS SALVO NO REGISTRADOR: ");
                 Serial_UART.print(i);
                 Serial_UART.print("\n");
                 Serial_UART.print(reg[i]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(reg1[i]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(reg2[i]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(reg3[i]);
                 Serial_UART.print(" , ");
                 Serial_UART.print(reg4[i]);
                 Serial_UART.print(".");
                 Serial_UART.print("\nSALVO COM SUCESSO!!!\n");
                 i++;
                 j = 0;
                 }
                 break;
                    }
                 }
             }
          /* else if(a == 52){ //CHAMA O MODO DE SETAR O REGISTRADOR PARA A POSIÇÃO QUE DESEJA
                   Serial_UART.print("\nMODO DE SETAR POSIÇÃO DO REGISTRADOR\n");
                   setar_reg(i);

           } */

             else { //Erro 1: Comando não cadastrado.
                  Serial_UART.print("\nERRO 1: \n");
                  }
           }


        status = rc522.Request(PICC_REQIDL, str);
        if (status == MI_OK)  // Cartão detectado
        {
            Serial_UART.print("\nRFID DETECTADO: ");
            Serial_UART.print(str[0]);
            Serial_UART.print(" , ");
            Serial_UART.print(str[1]);
            Serial_UART.print("\r\n");
        }

        status = rc522.Anticoll(str);
        memcopy(serNum, str, 5);
        if (status == MI_OK)
        {

            Serial_UART.print("ID: ");
            Serial_UART.print(serNum[0]);
            Serial_UART.print(" , ");
            Serial_UART.print(serNum[1]);
            Serial_UART.print(" , ");
            Serial_UART.print(serNum[2]);
            Serial_UART.print(" , ");
            Serial_UART.print(serNum[3]);
            Serial_UART.print(" , ");
            Serial_UART.print(serNum[4]);
            Serial_UART.print("\r\n");

            int SinalR  = 0;
            int Parada = 10;
            int contador; //variável de controle do loop
            for(contador = 0; contador <= Parada; contador++){
                if (serNum[0] == reg[contador] && serNum[1] == reg1[contador] && serNum[2] == reg2[contador] && serNum[3] == reg3[contador] && serNum[4] == reg4[contador])
                   {
                 ENTRADA[contador] = Func_Printar(ENTRADA[contador], contador);
                  SinalR = 1;
                  Parada = contador;

                   }
                  }

                if(SinalR == 0){
                   Tente_Serial();
                  }
      }

}
     rc522.Halt();
}



#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{


    if (UC0IFG & UCA0TXIFG)
    {
        //Pega os valores passados nos parâmetros da função
        byte* buf = Serial_UART.TxBuffer;
        volatile int* i = &Serial_UART.iTx;
        volatile byte* lock = &Serial_UART.lockTX;

        (*i)++;

        if (*i < Serial_UART.lenghtTX)
            UCA0TXBUF = buf[*i];


        else    //Coloca o valor para ser enviado no BUF do TX
        {
            while (UCA0STAT & UCBUSY);
            UC0IE &= ~UCA0TXIE;

            *lock = 0;

        }
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    if (UC0IFG & UCA0RXIFG)
    {

        //Pega os valores passados nos parâmetros da função
        byte* buf = Serial_UART.RxBuffer;
        volatile int* i = &Serial_UART.iRx;
        volatile byte* lock = &Serial_UART.lockRX;
        (*i)++;

        unsigned char rx = UCA0RXBUF;
        if(rx == '\0' || *i > Serial_UART.lenghtRX)
        {
            while (UCA0STAT & UCBUSY);
            UC0IE &= ~UCA0RXIE;
            *lock = 0;
        }

        else//(*i < Serial_UART.lenghtTX && buf[*(i - 1)] != '\0')
            buf[*i] = rx;
    }
}


