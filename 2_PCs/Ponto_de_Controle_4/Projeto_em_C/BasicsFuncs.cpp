// 
// 
// 

#include "BasicsFuncs.h"

void * memcopy(void * dest, const void * src, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		((char*)dest)[i] = ((char*)src)[i];

	}

	return dest;
}

void digitalWriteA(uint8_t pin, uint8_t BIT, uint8_t value)
{
	if (pin == 1)
		if (value)
			SetHigh(BIT, P1OUT);
		else
			SetLow(BIT, P1OUT);
	else
		if (value)
			SetHigh(BIT, P2OUT);
		else
			SetLow(BIT, P2OUT);
}

uint8_t digitalReadA(uint8_t pin, uint8_t BIT)
{
	if (pin == 1)
		return P1IN & BIT;
	else
		return P2IN & BIT;
}

void pinModeA(uint8_t pin, uint8_t BIT, uint8_t mode)
{
	if (mode)
		if (pin == 1)
			SetOut(BIT, P1DIR);
		else
			SetOut(BIT, P2DIR);

	else
		if (BIT == 2)
		{
			SetIn(BIT, P1DIR); SetPullUp(BIT, P1REN);
		}
		else
		{
			SetIn(BIT, P2DIR); SetPullUp(BIT, P2REN);
		}

}

void delayms(unsigned int x)
{
		// Periodo = 1us do clock -> Timer_A++ a cada 1us
		int i = 0; // Contador
		TA0CCR0 = 1000 - 1;
		TA0CTL = TASSEL_2 + ID_0 + MC_1;

		while (i<x)
		{
			while ((TA0CTL & TAIFG) == 0); // Só para quando TimerA chega a 1000. Só sai do while a cada 1ms;
			TA0CTL &= ~TAIFG;
			i++;
		}

		TA0CTL = MC_0;
}

int strsize(const char* str)
{
	volatile int len;
	for (len = 0; *(str++); len++);
	return len;
}

void itoa(long unsigned int value, char* result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgFEDCBA9876543210123456789ABCDEFghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

}
