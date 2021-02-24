/*
 * button.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

void button_init() {
    // BUTTON as interrupt
    P1IE |= BIT1;                     // Enable BUTTON interrupt
    P1IES |= BIT1;                    // H to L transition for interrupt
    P1REN |= BIT1;                    // Enable internal pull resistor
    P1OUT |= BIT1;                    // Set internal pull resistor as pull-up
    P1IFG &= ~BIT1;                   // Clear any pending ISRs
}

// PORT1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) P1_ISR(void)
#else
#error Compiler not supported!
#endif
{
    P1IFG = 0;                              // Clear Port1 IFG
    char interrupt_edge = P1IES & BIT1;
    if(interrupt_edge == 2) {               // Check interrupt edge
        button_pressed_callback();
        P1IES &= ~BIT1;               // L to H transition for interrupt
    }
    else {
        button_released_callback();
        P1IES |= BIT1;                // H to L transition for interrupt
    }
}
