// Serial_UART.h

#ifndef _SERIAL_UART_h
#define _SERIAL_UART_h

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include "arduino.h"
//#else
//	#include "WProgram.h"
//#endif

#include <msp430g2553.h>
#include "BasicsFuncs.h"
#define RXD	BIT1
#define	TXD	BIT2
typedef unsigned char byte;
class Serial_UARTClass
{
private:
	unsigned long baudRate;
	
public:
	//Armazenar os valores para enviar ou receber na memória
	byte* RxBuffer;
	byte* TxBuffer;

	// Contadores para os interrupts
	volatile int iRx;
	volatile int iTx; 

	//Travar na função e só sair quando receber tudo
	volatile byte lockRX;
	volatile byte lockTX;
	
	//Offset para inicio dos vetores
	int offsetRX;
	int offsetTX;

	//Tamanho dos vetores
	int lenghtRX;
	int lenghtTX;
	void init(unsigned long BaudRate = 9600UL);
	void read(byte* buff, int offset, int length);
	void write(byte* buff, int offset, int length);
	void print(const char* str);
	void print(unsigned char str);
	void print(unsigned char str, unsigned char Base);
	int readInt();
	unsigned char readByte();
	void readStr(char* buff, int offset, int length);
	bool available();
};

extern Serial_UARTClass Serial_UART;

#endif

