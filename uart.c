/*
 * uart.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

unsigned int rxbuf_size;
char rxbuf[50];

void uart_38400bps_init() {
    P1SEL1 &= ~(BIT6 | BIT7);                 // USCI_A0 UART operation
    P1SEL0 |= BIT6 | BIT7;

    // Configure UART
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;

    /// Baud Rate 38400bps
    /// UCOS16 = 1, UCBRx = 1, UCBRFx = 10, UCBRSx = 0
    UCA0BRW = 1;
    UCA0MCTLW = UCOS16 | UCBRF_10;

    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI

    UCA0IE |= UCRXIE;
}

void uart_putchar(char c) {
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF_L = c;
}

void uart_puts(const char *str) {
    uart_print(str);
    uart_putchar('\r');
    uart_putchar('\n');
}

void uart_print(const char *str) {
    if (str) {
        char *p = (char *)str;
        while (*p) {
            uart_putchar(*p++);
        }
    }
}

void uart_write(const char *data, unsigned int len) {
    unsigned int i;
    for (i = 0; i < len; i++) {
        uart_putchar(data[i]);
    }
}

// UART interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
            if (rxbuf_size >= sizeof rxbuf) {
                rxbuf_size = 0;
            }
            rxbuf[rxbuf_size++] = UCA0RXBUF_L;
            
            /// TODO: handle uart input

            
            break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
    }
}
