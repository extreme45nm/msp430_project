#include <msp430.h> 

/*
 * main.c
 */

#define LED1 BIT0
#define LED2 BIT6

void configClock(){
	DCOCTL = 0;
	BCSCTL1 = 0b1000000;
	BCSCTL2 |= DIVS_3;
	DCOCTL = 0b0100000;
}

void configTimer(){

	TA0CTL = TASSEL_2 + ID_0 + MC_1 + TACLR;
	TA0CCR0 = 12500;
	TA0CCTL0 = CCIE;

	TA1CTL = TASSEL_2 + ID_3 + MC_1 + TACLR;
	TA1CCR0 = 12500;
	TA1CCTL0 = CCIE;
}

void configPort(){

	P1OUT = 0x00;
	P1DIR |= LED1 + LED2;

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void t0a0_isr(){

	P1OUT ^= LED1;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void t1a0_isr(){
	P1OUT ^= LED2;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    configClock();
    configTimer();
    configPort();

    _BIS_SR(CPUOFF + GIE);
    while(1){}
}
