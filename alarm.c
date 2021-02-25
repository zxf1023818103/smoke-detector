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

void alarm_on() {
    //zigbee_send("alarm on");
    alarm = 1;
}

void alarm_off() {
    zigbee_send("alarm off");
    alarm = 0;
}

int alarm_status() {
    return alarm;
}

void alarm_silent() {
    silent_cycles = settings_get_alarm_silent_cycles();
}

void alarm_loop() {
    if (silent_cycles) {
        alarm = 0;
        silent_cycles--;
    }

    if (alarm) {
        if (!buzzer_status()) {
            buzzer_on();
        }
        else {
            buzzer_off();
            zigbee_send("alarm on");
        }
    }
    else {
        if (!test) {
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
