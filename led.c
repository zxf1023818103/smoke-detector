/*
 * led.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>

void led_on() {
    P2DIR |= BIT6;
    P2OUT |= BIT6;
    P2REN &= ~BIT6;
}

void led_off() {
    P2DIR &= ~BIT6;
    P2OUT &= ~BIT6;
    P2REN |= BIT6;
}

void led_init() {
    led_off();
}

char led_status() {
    return P2OUT & BIT6;
}

void led_toggle() {
    P2DIR |= BIT6;
    P2OUT ^= BIT6;
}

void ir_on() {
    P1DIR |= BIT5;
    P1OUT |= BIT5;
    P2REN |= BIT6;
}

void ir_off() {
    P1DIR &= ~BIT5;
    P1OUT &= ~BIT5;
}

void ir_init() {
    ir_off();
}

void ir_toggle() {
    P1DIR |= BIT5;
    P1OUT ^= BIT5;
}
