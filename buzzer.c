/*
 * buzzer.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>

static int buzzer;

void buzzer_init() {
    TB1CTL &= ~MC__UP;
    TB1CCTL0 |= CCIE;                   // TBCCR0 interrupt enabled
    TB1CCR0 = 25;
    TB1CTL = TBSSEL__SMCLK | MC__UP;    // SMCLK, UP mode
    TB1EX0 |= TBIDEX_4;
}

void buzzer_on() {
    buzzer = 1;
    P2DIR |= BIT7;
    TB1CTL |= MC__UP;
}

void buzzer_off() {
    buzzer = 0;
    P2DIR &= ~BIT7;
    TB1CTL &= ~MC__UP;
}

int buzzer_status() {
    return buzzer;
}

// Timer1_B0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P2OUT ^= BIT7;
}
