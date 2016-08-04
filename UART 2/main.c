#include "msp430g2553.h"

#define TXLED BIT0
#define RXLED BIT6
#define TXD	BIT2
#define RXD BIT1

const char string[] = {"Hello World!\r\n"};
unsigned int i;


void configClock(){
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
}


void configPort(){
	P2DIR = 0xff;
	P2OUT &= 0x00;

	P1SEL |= RXD + TXD;
	P1SEL2 |= RXR + TXD;

	P1DIR |= RXLED + TXLED;
	P1OUT &= 0x00;
}


void configUART(){
	UCA0CTL1 |= UCSSEL_2;

	UCA0BR0 = 0x08;
	UCA0BR1 = 0x00;

	UCA00MCTL = UCBRS2 + UBCRS0;
	UCA0CTL1 &= ~UCSWRST;
	UC0IE |= UCA0RXIE;
}


#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_isr(){
	P1OUT |= RXLED;
	if(UCA0RXBUF == 'a'){
		i = 0;
		UC0IE |= UCA0TXIE;
		UCA0TXBUF = string[i++];
	}
	P1OUT &= ~RXLED;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_isr(){
	P1OUT |= TXLED;
	UCA0TXBUF = string[i++];
	if(i == sizeof string - 1){
		UC0IE &= ~UCA0TXIE;
	}
	P1OUT &= ~TXLED;

}

int main(){

	WDTCTL = WDTHOLD + WDTPW;
	configClock();
	configPort();
	configUART();

	__bis_SR_register(CPUOFF + GIE);
	while(1){}
}
