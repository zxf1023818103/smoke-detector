/*
 * alarm.c
 *
 *  Created on: 2021/2/24
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

char test;
char alarm;

void alarm_test() {
    test = 1;
}

void alarm_on() {
    zigbee_send_alarm();
    alarm = 1;
}

void alarm_off() {
    alarm = 0;
}

void alarm_loop() {
    if (alarm) {
        if (!buzzer_status()) {
            buzzer_on();
        }
        else {
            buzzer_off();
            zigbee_send_alarm();
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
