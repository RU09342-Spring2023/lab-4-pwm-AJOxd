#include <msp430.h> 
void LEDsetup(void);
void ButtonSetup(void);
void TimerA0Setup(void);
void TimerA1Setup(void);
unsigned int DutyCycle = 1000;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	LEDsetup();
	ButtonSetup();
	TimerA0Setup();
	TimerA1Setup();
	__bis_SR_register(LPM0_bits + GIE);
}

void LEDsetup(){
    P1OUT &= ~BIT0; // CLEAR P1.0
    P1DIR |= BITO; // SET P1.0 AS OUTPUT
    P6OUT &= ~BIT6; // CLEAR P6.6
    P6DIR |= BIT6; // SET P6.0 AS OUTPUT
}
void ButtonSetup(){
    // P2.3 AND P4.1
    P2OUT |= BIT3; // PULL UP REGI 2.3
    P2REN |= BIT3; // EN P2.3
    P2IES &= ~BIT3; // P2.3 LH EDGE
    P2IE |= BIT3; // EN P2.3 INTER
    P4OUT |= BIT1; // PULL UP REGI 4.1
    P4REN |= BIT1; // EN P4.1
    P4IES &= ~BIT1; // P4.1 LH EDGE
    P4IE |= BIT1; // EN P4.1 INTER
}
void TimerA0Setup(){
    TB0CTL = TBSSEL_2 | MC__UP | TBCLR | TBIE;
    TB0CCTL1 |= CCIE;
    TB0CCTL2 |= CCIE;
    TB0CCR0 = DutyCycle;
    TB0CCR1 = DutyCycle/2;
    TB0CCR2 = DutyCycle/2;
}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
    P2IFG &= ~BIT3;
    if(TB0CCR1 >= 1000)
        TB0CCR2 = 0;
    else
        TB0CCR2 += 150;
}
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void){
    P4IFG &= ~BIT1;
    if(TB0CCR1 >= 1000)
        TB0CCR2 = 0;
    else
        TB0CCR2 += 150;
}
#if defined(TI_COMPILER_VERSION) || defined(IAR_SYSTEMS_ICC)
#pragma vector=TIMER0_B1_VECTOR
interrupt void TIMER0_B1_ISR(void)
#elif defined(GNUC)
void attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV, TB0IV-TBIFG))
    {
    case TB0IV_zero;
        break;
    case TB0IV_TBCCR1;
        P1OUT &= ~BIT0;
        break;
    case TB0IV_TBCCR2;
        P6OUT &= ~BIT6;
    case TB0IV_TBIFG;
        P1OUT |= BIT0;
        P6OUT |= BIT6;
        break;
    default;
        break;

    }
}

