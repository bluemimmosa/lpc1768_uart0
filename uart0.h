#ifndef SETUP_H
#define SETUP_H

#include "lpc17xx.h"


enum PERIPHERALS
{ 
		PCTIM0=1,
		PCTIM1,
		PCUART0,
		PCUART1,
		PCPWM1=6,
		PCI2C0,
		PCSPI,
		PCRTC,
		PCSSP1,
		PCADC=12,
		PCCAN1,
		PCCAN2,
		PCGPIO,
		PCRIT,
		PCMCPWM,
		PCQEI,
		PCI2C1,
		PCSSP0=21,
		PCTIM2,
		PCTIM3,
		PCUART2,
		PCUART3, 
		PCI2C2, 
		PCI2S, 
		PCGPDMA=29,
		PCENET,
		PCUSB 
};

enum MODE
{
		INPUT,
		OUTPUT
};


// LCR register enumeration
enum _UART_LCR
{
	WLS0 = 0,
	WLS1,
	STOP_BIT,
	PAIRITY_ENABLE,
	PAIRITY_SELECT_BIT0,
	PAIRITY_SELECT_BIT1,
	BREAK_CONTROL,
	DLAB
};	

// FCR register enumeration
enum _UART_FCR
{
	FIFOEN,
	RXFIFO_CLR,
	TXFIFO_CLR
};

enum _UART_IER
{
	RBR_IE=0,
	THR_IE,
	RX_LINE_IE,
	MS_IE,
	CTS_IE=7
};

enum IRQ_CAT
{
	IRQ_OFF,
	IRQ_ON
};

#endif

extern void uart0_init( uint32_t, uint32_t,  enum IRQ_CAT );
extern void uart0_tx_string(uint8_t * );
