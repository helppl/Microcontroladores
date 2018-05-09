#include <msp430g2533.h>
#include <Serial_UART.h>
#include <Serial_SPI.h>
#include <Rc522.h>

void SendCommand(const char* command, const int timeout)
{
    // Envio dos comandos AT para o modulo
    Serial_UART.print(command);
    delayms(timeout);
}

#define CS BIT0
#define NRSTDP BIT3
Rc522 rc522(CS, NRSTDP);
unsigned char serNum[5];
char recv[9] = {0};

#define TEST 0

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT


    Serial_UART.init(9600);
  delayms(200);
  pinModeA(1,BIT0, OUTPUT);
  digitalWriteA(1,BIT0, LOW);
  SendCommand("AT+RST\r\n", 2000);
  delayms(4000);

  delayms(1000);
  //SendCommand("AT+CWJAP=\"VIVO-0100\",\"5760500100\"\r\n", 2000);
  delayms(1000);
  SendCommand("AT+CWMODE=1\r\n", 1000);
  // Mostra o endereco IP
  delayms(1000);
  SendCommand("AT+CIFSR\r\n", 1000);

  // Se conecta ao servidor pela porta 65410
  SendCommand("AT+CIPSTART=\"TCP\",\"192.168.15.30\",65410\r\n", 2000);
  delayms(4000);

  Serial_SPI.init();
  digitalWriteA(1, CS, LOW);

  rc522.Init();
  __bis_SR_register(GIE);
  while (1)
  {
    unsigned char status;
    unsigned char str[MAX_LEN];

    status = rc522.Request(PICC_REQIDL, str);

    status = rc522.Anticoll(str);
    memcopy(serNum, str, 5);
    if (status == MI_OK)
    {

      char rfid[] = { serNum[0], serNum[1] ,serNum[2] ,serNum[3] ,serNum[4] };
      SendCommand("AT+CIPSEND=5\r\n",1000);
      Serial_UART.print(rfid);
      delayms(1000);

      Serial_UART.readStr(recv, 0, 9);

      char result = recv[7];
      if (result == '1')
      {
        digitalWriteA(1, BIT0, HIGH);
        delayms(5000);
        digitalWriteA(1, BIT0, LOW);
      }
      else
      {
        digitalWriteA(1, BIT0, LOW);
      }
    }

    rc522.Halt();
  }
}
