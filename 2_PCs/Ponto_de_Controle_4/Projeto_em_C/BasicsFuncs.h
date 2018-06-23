#include <msp430g2553.h>


#define SetOut(pin,x)		 (x |= pin)
#define SetIn(pin,x)		 (x &= ~pin)
#define SetPullUp(pin,x)	 (x |= pin)
#define SetPullDown(pin,x)   (x &= ~pin)
#define SetHigh(pin,x)		 (x |= pin)
#define SetLow(pin,x)		 (x &= ~pin)
#define SwitchValue(pin,X)	 (x ^= pin)

#define HIGH 1
#define LOW 0

#define OUTPUT 1
#define INPUT 0

typedef unsigned char byte;
typedef byte uint8_t;
typedef unsigned int size_t;
void digitalWriteA(uint8_t pin, uint8_t BIT, uint8_t value);
uint8_t digitalReadA(uint8_t pin, uint8_t BIT);
void pinModeA(uint8_t pin, uint8_t BIT, uint8_t mode);
void* memcopy(void *dest, const void *src, size_t size);
void delayms(volatile unsigned int x);
int strsize(const char* str);
void itoa(long unsigned int value, char * result, int base);
int Func_Printar(int ENTRADA);
