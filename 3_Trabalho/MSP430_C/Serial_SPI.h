// Serial_SPI.h

#ifndef _SERIAL_SPI_h
#define _SERIAL_SPI_h

#include <msp430g2553.h>

class Serial_SPIClass
{
 protected:


 public:
	void init();
	unsigned char transfer(unsigned char data);
};

extern Serial_SPIClass Serial_SPI;

#endif

