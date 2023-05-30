#include <msp430.h>

int main(void)
{
  P1DIR |= BIT2;             // P1.2 set for output
  P1SEL |= BIT2;             // select TA0.1 output signal
  P6SEL |= BIT0;             //select A0 for input signal

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer

  //ADC
  REFCTL0 &= ~REFMSTR;
  ADC12CTL1 = ADC12SHP;
  ADC12CTL0 = ADC12REF2_5V + ADC12SHT02 + ADC12REFON + ADC12ON ;
  ADC12MCTL0 = ADC12SREF_1;
  __delay_cycles(300);
  ADC12CTL0 |= ADC12ENC;

  //PWM
  TA0CCR0 = 20000; //2ms PWM
  TA0CCTL1 = OUTMOD_7; //Reset/Set Mode
  TA0CTL = TASSEL_2 | MC_1;

  while (1)
  {
    ADC12CTL0 |= ADC12SC;                   // Start
    while (ADC12CTL1 & ADC12BUSY);

    int potvalue = ADC12MEM0;
    TA0CCR1 = (int) (350+ (1.95 * potvalue));
    __delay_cycles(20000);
  }
}
