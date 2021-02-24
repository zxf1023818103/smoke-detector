/*
 * zigbee.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include <string.h>
#include "smokedetector.h"

void zigbee_send(const char *str) {
    uart_print("AT+UT_SEND=02,");
    uart_print(str);
    uart_putchar('\r');
    uart_putchar('\n');
}

void zigbee_send_alarm() {
    zigbee_send("alarm\r\n");
}

void zigbee_println(unsigned short value) {
    char str[7];
    unsigned int i = 0;
    for (;;) {
        str[i++] = value % 10 + '0';
        value /= 10;
        if (value == 0) {
            break;
        }
    }
    uart_print("AT+UT_SEND=02,");
    while (i--) {
        uart_putchar(str[i]);
    }
    uart_putchar('\r');
    uart_putchar('\n');
    uart_putchar('\r');
    uart_putchar('\n');
}

void uart_newline_callback(char *str, unsigned int len) {
    int ok = 0;
    if (strncmp("test\r\n", str, len) == 0) {
        alarm_test();
        ok = 1;
    }
    else if (strncmp("alarm on\r\n", str, len) == 0) {
        alarm_on();
        ok = 1;
    }
    else if (strncmp("alarm off\r\n", str, len) == 0) {
        alarm_off();
        ok = 1;
    }
    if (ok) {
        zigbee_send("ok\r\n");
    }
}
