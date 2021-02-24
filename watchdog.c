/*
 * watchdog.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

char watchdog_enabled;
extern char timer_500ms_enabled;

void watchdog_clear() {
    WDTCTL = WDTPW | WDTCNTCL | WDTSSEL__ACLK | WDTIS_4;
}

void watchdog_init() {
    watchdog_clear();
    watchdog_enabled = 1;
    if (!timer_500ms_enabled) {
        timer_500ms_init();
    }
}
