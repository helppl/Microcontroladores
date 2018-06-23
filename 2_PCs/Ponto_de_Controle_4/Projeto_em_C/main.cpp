#include <msp430g2533.h>
#include <Serial_UART.h>
#include <Serial_SPI.h>
#include <Rc522.h>
#include <string.h>

#define LED1 BIT0
#define CS BIT0
#define NRSTDP BIT3
Rc522 rc522(CS, NRSTDP);
unsigned char serNum[5];

int REGISTRADOR1[10];
int REGISTRADOR2[10];
int REGISTRADOR3[10];
int REGISTRADOR4[10];
int ENTRADA1 = 0;
int ENTRADA2 = 0;
//char recv[9] = {0};
char nome_cliente[16];

 // ABRIR O RELÊ
  void Abrir_Porta(){
      while(1){
      digitalWriteA(1, LED1, HIGH); //Abrir o rele
      delayms(2000);
      break;
      }
  }

 // FECHAR O RELÊ
  void Fechar_Porta(){
      while(1){
      digitalWriteA(1, LED1, LOW); //Ativar o relê
      break;
      }
     }



 /* ********************************************************************************
     *  Funções para transmissão serial dos dados para serem salvos numa pasta   ! *
     ********************************************************************************** */

 void Entrada_Serial()
     {
     while(1){
     Serial_UART.print("BEM VINDO,  "); // printar a msg de acesso
     Serial_UART.print(nome_cliente); // printar na parte de baixo do display
     Serial_UART.print("\r\n");
     break;
     }
 }


 void Saida_Serial(){
     while(1){
      Serial_UART.print("VOLTE SEMPRE, "); // printar a msg de acesso
      Serial_UART.print(nome_cliente); // printar na parte de baixo do display
      Serial_UART.print("\r\n");
      break;
     }


 }

 void Tente_Serial(){
     while(1){
     Serial_UART.print("ACESSO NEGADO..."); // printar a msg de acesso
     Serial_UART.print("TENTE NOVAMENTE"); // printar na parte de baixo do display
     Serial_UART.print("\r\n");
     break;
     }

}

 void Tela_Espera(){
      while(1){
      Serial_UART.print("PASSE O CARTAO");
      Serial_UART.print("\r\n");
      break;
      }

 }

//FUNÇÃO GERARAL PARA PRINTAR
int Func_Printar(volatile int ENTRADA){
    //ENTRADA GENERICA

    if(ENTRADA == 0){
       //   lcd.setCursor( 3 , 0 );
        //  lcd.print("BEM VINDO,"); // printar a msg de acesso
        //  lcd.setCursor( 4 , 1 ); //ajustando a posição
        //  lcd.print(nome_cliente); // printar na parte de baixo do display
          Abrir_Porta();
          Fechar_Porta();
          Entrada_Serial();
          ENTRADA = 1;
         // Serial_UART.print("BEM VINDO,"); // printar a msg de acesso
         // Serial_UART.print(nome_cliente); // printar na parte de baixo do display

         }

     else if (ENTRADA == 1){
       //   lcd.setCursor( 2 , 0 );
      //    lcd.print("VOLTE SEMPRE,"); // printar a msg de acesso
      //    lcd.setCursor( 4 , 1 ); //ajustando a posição
      //    lcd.print(nome_cliente);
            Abrir_Porta();
            Fechar_Porta();
            Saida_Serial();
            ENTRADA = 0;
        //    Serial_UART.print("VOLTE SEMPRE,"); // printar a msg de acesso
       //   Serial_UART.print(nome_cliente); // printar na parte de baixo do display

          }

      else if (ENTRADA == 2) {
          Tente_Serial();
          Serial_UART.print("ACESSO NEGADO...");
          Serial_UART.print("TENTE NOVAMENTE");


        //   lcd.setCursor(0, 0); // ajustando a posição
       //    lcd.print("ACESSO NEGADO..."); //Printar a msg quando o acesso for negado
       //    lcd.setCursor(0 , 1); // ajustando a posição de baixo
       //    lcd.print("TENTE NOVAMENTE");
        //   delay(5000);  //5s
           }
      else {
          Serial_UART.print("ERROR 3");
           }

    return (ENTRADA);

 }

//FUNÇÃO PRINCIPAL

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT


    Serial_UART.init(9600);
    Serial_UART.print("RFID LOADING");
    Serial_UART.print("\r\n");
    Serial_SPI.init();
    digitalWriteA(1, CS, LOW);
    pinModeA(1, LED1, OUTPUT);
    digitalWriteA(1, LED1, LOW);
    rc522.Init();




    while (1)
    {

        unsigned char status;
        unsigned char str[MAX_LEN];

        status = rc522.Request(PICC_REQIDL, str);
        if (status == MI_OK)  // Cartão detectado
        {
            Serial_UART.print("RFID detectado: ");
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

            if(serNum[0] == 0xBB && serNum[1] == 0x01 && serNum[2] == 0xB5 && serNum[3] == 0xA9 && serNum[4] == 0xA6) //SR.FELICIO
               {
                 strcpy(nome_cliente, "SR. FELICIO");
                  ENTRADA1 = Func_Printar(ENTRADA1);
                  }
               else if (serNum[0] == 253 && serNum[1] == 125 && serNum[2] == 181 && serNum[3] == 21 && serNum[4] == 32) //SR. BAPISTA
                  {
                  strcpy(nome_cliente, "SR. BAPTISTA");
                  ENTRADA2 = Func_Printar(ENTRADA2);

                  }
               else
                  {
                  Func_Printar(2);
                  }
        }

        rc522.Halt();
    }

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


