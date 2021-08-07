/*
 * main.c
 *
 *  Created on: 2021/2/24
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

char test_button;

void setup() {
    clock_1mhz_init();
    software_trim();
    watchdog_init();
    timer_500ms_init();
    ir_on();
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

    __enable_interrupt();
}

void button_pressed_callback() {
    if (!alarm_status()) {
        test_button = 1;
    }
    alarm_on();
}

void button_released_callback() {
    alarm_off();
    if (test_button) {
        test_button = 0;
    }
    else {
        alarm_silent();
    }
}

static unsigned short adc_value;

void adc_sample_callback(unsigned short value) {
    adc_value = value;
    if (settings_get_adc_value_report_enabled()) {
        /// TODO: report adc value
    }
    if (value > settings_get_sensitivity()) {
        alarm_on();
    }
}

void timer_500ms_callback() {
    alarm_loop();
    adc_sample();
}
