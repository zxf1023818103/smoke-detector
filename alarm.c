/*
 * alarm.c
 *
 *  Created on: 2021/2/24
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

static char test;
static char alarm;
static unsigned int silent_cycles;

void alarm_test() {
    test = 1;
}

char alarm_is_test() {
    return test;
}

void alarm_on() {
    alarm = 1;
}

void alarm_off() {
    zigbee_send(settings_get_alarm_report_channel(), "alarm=0");
    alarm = 0;
}

int alarm_status() {
    return alarm;
}

void alarm_silent() {
    silent_cycles = settings_get_alarm_silent_cycles();
}

unsigned int alarm_get_remain_silent_cycles() {
    return silent_cycles;
}

void alarm_loop() {
    if (silent_cycles) {
        alarm = 0;
        silent_cycles--;
    }

    if (alarm) {
        if (!buzzer_status()) {
            led_on();
            buzzer_on();
        }
        else {
            buzzer_off();
            led_off();
            zigbee_send(settings_get_alarm_report_channel(), "alarm=1");
        }
    }
    else {
        if (!test) {
            led_off();
            buzzer_off();
        }
    }

    if (test) {
        buzzer_on();
        test--;
    }
    else {
        if (!alarm) {
            buzzer_off();
        }
    }
}
