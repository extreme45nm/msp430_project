#include <msp430.h> 

/*
 * main.c
 */
#define LED1 BIT0
#define LED2 BIT6
#define SWT1 BIT3

void configClock(){

	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
}

void configTimer(){

	TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR;
	TA0CCR0 = 62500 - 1;
	TA0CCTL0 = CCIE;
}

void configPort(){

	P1OUT = 0x00;
	P1DIR |= LED1 + LED2;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void t0a0_isr(){

	P1OUT ^= LED1;
}


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    configClock();
    configTimer();
    configPort();

    _BIS_SR(CPUOFF + GIE);

    while(1){}
}
