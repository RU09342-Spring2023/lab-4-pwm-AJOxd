#include <msp430.h> 
void gpioInit();
void timerInit();
#define red 0
#define blue 1
#define green 2
char LED = red;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	gpioInit();
	timerInit();
	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();
}
void gpioInit(){
    P6DIR |= BIT0;
    P6OUT &= ~BIT0;
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;
    P6IE |= BIT0;
    P6DIR |= BIT1;
    P6OUT &= ~BIT1;
    P6SEL0 |= BIT1;
    P6SEL1 &= ~BIT1;
    P6IE |= BIT1;
    P6DIR |= BIT2;
    P6OUT &= ~BIT2;
    P6SEL0 |= BIT2;
    P6SEL1 &= ~BIT2;
    P6IE |= BIT2;
}
void timerInit(){
    TB3CCR0 = 1000-1;
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;
    TB3CCTL1 = OUTMOD_7;
    TB3CCTL2 = OUTMOD_7;
    TB3CCTL3 = OUTMOD_7;

    TB3CCR1 = 1000-1;
    TB3CCR2 = 0;
    TB3CCR3 = 0;

    TB1CTL = TBSSEL_1 |MC_2 | ID_3 | TBCLR | TBIE;
    TB1CCTL0 |= CCIE;
    TB1CCR0 = 1;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void TIMER1_B0_ISR(void)
{
    switch(LED)
    {
    case red:
        TB3CCR1--;
        TB3CCR2++;
        if(TB3CCR1 == 0){
            LED = blue;
        }
        break;
    case blue:
        TB3CCR2--;
        TB3CCR3++;
        if(TB3CCR2 ==0){
            LED = green;
        }
        break;
    case green:
        TB3CCR3--;
        TB3CCR1++;
        if(TB3CCR3 ==0){
            LED = red;
        }
    }
}
