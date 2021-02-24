/*
 * adc.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

void adc_init() {
    // Configure ADC A3 pin
    P1SEL0 |= BIT3;
    P1SEL1 |= BIT3;

    P1DIR |= BIT4;
    P1OUT |= BIT4;

    P1DIR |= BIT2;
    P1OUT &= ~BIT2;

    // Configure ADC10
    ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
    ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_3;                                   // A3 ADC input select; Vref=AVCC
    ADCIE |= ADCIE0;
}

void adc_sample() {
    ADCCTL0 |= ADCENC | ADCSC;                               // Sampling and conversion start
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_ADCIFG:
            adc_sample_callback(ADCMEM0);
            break;
        default:
            break;
    }
}
