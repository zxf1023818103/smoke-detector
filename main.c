/*
 * main.c
 *
 *  Created on: 2021/2/24
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

void setup() {
    clock_1mhz_init();
    software_trim();
    watchdog_init();
    timer_500ms_init();
    //ir_on();
    led_off();
    uart_38400bps_init();
    buzzer_init();
    adc_init();
    led_init();
    button_init();
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                                // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;

    setup();

    ir_on();

    __enable_interrupt();
}

void button_pressed_callback() {
    alarm_on();
}

void button_released_callback() {
    alarm_off();
}

void adc_sample_callback(unsigned short adc_value) {
    //zigbee_println(adc_value);
    if (adc_value > 800) {
        alarm_on();
    }
}

void timer_500ms_callback() {
    alarm_loop();
    adc_sample();
}
