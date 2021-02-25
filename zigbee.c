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
    uart_putchar('\r');
    uart_putchar('\n');
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
    if (strncmp("test\r\n", str, len) == 0) {
        alarm_test();
        zigbee_send("ok");
    }
    else if (strncmp("alarm on\r\n", str, len) == 0) {
        alarm_on();
    }
    else if (strncmp("alarm off\r\n", str, len) == 0) {
        alarm_off();
    }
    else if (strncmp("alarm status\r\n", str, len) == 0) {
        zigbee_send(alarm_status() ? "alarm on" : "alarm off");
    }
    else if (strncmp("sensitivity status\r\n", str, len) == 0) {
        unsigned int sensitivity = settings_get_sensitivity();
        zigbee_send(sensitivity == 700 ? "sensitivity=0" : (sensitivity == 800 ? "sensitivity=1" : (sensitivity == 900 ? "sensitivity=2" : "sensitivity=3")));
    }
    else if (strncmp("sensitivity=0\r\n", str, len) == 0) {
        settings_set_sensitivity(700);
        zigbee_send("ok");
    }
    else if (strncmp("sensitivity=1\r\n", str, len) == 0) {
        settings_set_sensitivity(800);
        zigbee_send("ok");
    }
    else if (strncmp("sensitivity=2\r\n", str, len) == 0) {
        settings_set_sensitivity(900);
        zigbee_send("ok");
    }
    else if (strncmp("sensitivity=3\r\n", str, len) == 0) {
        settings_set_sensitivity(1000);
        zigbee_send("ok");
    }
    else if (strncmp("adc_output_enabled=0\r\n", str, len) == 0) {
        settings_set_adc_output_enabled(0);
        zigbee_send("ok");
    }
    else if (strncmp("adc_output_enabled=1\r\n", str, len) == 0) {
        settings_set_adc_output_enabled(1);
        zigbee_send("ok");
    }
}
