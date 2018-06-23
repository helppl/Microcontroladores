// 
// 
// 

#include "Serial_SPI.h"

void Serial_SPIClass::init()
{
	UCB0CTL1 = UCSWRST | UCSSEL_2;      // Put USCI in reset mode, source USCI clock from SMCLK
	UCB0CTL0 = UCCKPH | UCMSB | UCSYNC | UCMST;    // Use SPI MODE 0 - CPOL=0 CPHA=0
	//P1DIR |= BIT5;
	
	P1SEL |= BIT5 + BIT6 + BIT7;                         // Configures SPI
	P1SEL2 |= BIT5 + BIT6 + BIT7;
													 /* Set pins to SPI mode. */
	UCB0BR0 = 2;   
	UCB0BR1 = 0;

	UCB0CTL1 &= ~UCSWRST;			    // release USCI for operation

}

unsigned char Serial_SPIClass::transfer(unsigned char data)
{
	UCB0TXBUF = data; // setting TXBUF clears the TXIFG flag
	while (UCB0STAT & UCBUSY);

	return UCB0RXBUF; // reading clears RXIFG flag
}


Serial_SPIClass Serial_SPI;

