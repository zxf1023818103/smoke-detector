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

static int isblank(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static int trim(char **begin, char **end) {
    if (*begin >= *end) {
        return 0;
    }

    while (isblank(**begin) && *begin != *end) {
        ++*begin;
    }
    if (*begin != *end) {
        --*end;
        while (isblank(**end)) {
            --*end;
        }
        ++*end;
        return 1;
    }
    else {
        return 0;
    }
}

static char *invalid_value_string = "ERROR: Invalid value";

static void process_option(char *key_begin, char *key_end, char *value_begin, char *value_end) {
    *key_end = 0;
    *value_end = 0;
    if (*value_begin == '?') {
        const char *value = attribute_read_callback(key_begin);
        if (value) {
            zigbee_send(value);
        }
        else {
            zigbee_send(invalid_value_string);
        }
    }
    else {
        int result = attribute_write_callback(key_begin, value_begin);
        switch (result) {
        case 0:
            zigbee_send("ok");
            break;
        case 1:
            zigbee_send("ERROR: Invalid key or unsupported operation");
            break;
        case 2:
            zigbee_send(invalid_value_string);
            break;
        default:
            break;
        }
    }
}

static void process_command(char *command_begin, char *command_end) {
    *command_end = 0;
    command_received_callback(command_begin);
}

void uart_newline_callback(char *str, unsigned int len) {
    char *equals = strchr(str, '=');
    if (equals) {
        char *key_begin = str, *key_end = equals;
        if (trim(&key_begin, &key_end)) {
            char *value_begin = equals + 1, *value_end = str + len;
            if (trim(&value_begin, &value_end)) {
                process_option(key_begin, key_end, value_begin, value_end);
                return;
            }
            else {
                zigbee_send("ERROR: excepted option value");
            }
        }
        else {
            zigbee_send("ERROR: excepted option name");
        }
    }
    else {
        char *command_begin = str, *command_end = str + len;
        if (trim(&command_begin, &command_end)) {
            process_command(command_begin, command_end);
        }
        else {
            return;
        }
    }
}
