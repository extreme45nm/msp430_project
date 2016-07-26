/* dcodemo_g2211: demonstrates configuration of the DCO by showing changing LED
 * flash frequency.  The LED flash uses the same delay loop, but the speed at
 * which the loop runs depends on the DCO frequency running MCLK.
 */

#include <msp430g2553.h>

#define LED1   BIT0
#define BTN1   BIT3
#define LED2   BIT6

void configClock(){

	DCOCTL = 0;		// clear stepp
	BCSCTL1 = CALBC1_1MHZ;	//
	BCSCTL2 = 0b00000110;
	DCOCTL  = 0b01100000;


}

void configTimer(){

	TA0CTL = TASSEL_2 + ID_3 + MC_1;
	TA0CCR0 = 15625;
	TA0CCTL0 = CCIE;


}

void configPort(){

//	P1OUT = 0;
	P1DIR |= LED1 + LED2;
	P1IN |= BTN1;

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void t0a0_isr(){

	P1OUT ^= LED1;
}

void main(){

	configPort();
	configClock();
	configTimer();

	_BIS_SR(CPUOFF + GIE);

	while(1){}

}
