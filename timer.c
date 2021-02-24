/*
 * timer.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

extern char watchdog_enabled;
extern void watchdog_clear();
char timer_500ms_enabled;

void timer_500ms_init() {
    TB0CTL &= ~MC__UPDOWN;
    TB0CCTL0 = CCIE;
    TB0CTL = TBSSEL__ACLK | MC__CONTINUOUS | CNTL__12 | ID_2;
    timer_500ms_enabled = 1;
}

// Timer0_B0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (watchdog_enabled) {
        watchdog_clear();
    }
    if (timer_500ms_enabled) {
        timer_500ms_callback();
    }
}
