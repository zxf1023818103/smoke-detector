/*
 * zigbee.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include <string.h>
#include "smokedetector.h"

void zigbee_send(unsigned int channel, const char *str) {
    char buffer[7];
    uart_print(my_itoa(channel, buffer));
    uart_putchar(' ');
    uart_puts(str);
}

void zigbee_send_read_request_response(unsigned int channel, const char *str) {
    char buffer[7];
    uart_print(my_itoa(channel, buffer));
    uart_putchar(' ');
    uart_print("OK");
    uart_putchar(' ');
    uart_puts(str);
}

void zigbee_report_attribute(unsigned int channel, const char *attribute, unsigned short value) {
    (void) attribute;
    char buffer[7];
    zigbee_send_read_request_response(channel, my_itoa(value, buffer));
}

// Use GCC built-in function.
#ifndef __GNUC__ 

int isblank(int c) {
    return c == ' ' || c == '\t';
}

#endif

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

static void process_attribute(unsigned int channel, char *key_begin, char *key_end, char *value_begin, char *value_end) {
    *key_end = 0;
    *value_end = 0;
    if (*value_begin == '?') {
        const char *value = attribute_read_callback(channel, key_begin);
        if (value) {
            zigbee_send_read_request_response(channel, value);
        }
        else {
            zigbee_send(channel, "ERROR invalid key");
        }
    }
    else {
        int result = attribute_write_callback(channel, key_begin, value_begin);
        switch (result) {
        case 0:
            zigbee_send(channel, "OK");
            break;
        case 1:
            zigbee_send(channel, "ERROR invalid key or unsupported operation");
            break;
        case 2:
            zigbee_send(channel, "ERROR invalid value");
            break;
        default:
            break;
        }
    }
}

static void process_command(unsigned int channel, char *command_begin, char *command_end) {
    *command_end = 0;
    command_received_callback(channel, command_begin);
}

void uart_newline_callback(char *str, unsigned int len) {
    char *begin = str, *end = str + len;

    end--;
    while (*end == '\r' || *end == '\n') {
        end--;
    }
    end++;

    while (isblank(*begin) && begin < end) {
        begin++;
    }

    char *blank = my_strchr(begin, ' ');
    if (blank) {
        char *channel_begin = begin, *channel_end = begin;
        while (is_digit(*channel_end)) {
            channel_end++;
        }
        if (channel_end == blank) {
            *channel_end = 0;

            unsigned int channel;
            my_atoi(channel_begin, &channel);

            char *equals = my_strchr(blank + 1, '=');
            if (equals) {
                char *key_begin = blank + 1, *key_end = equals;
                if (trim(&key_begin, &key_end)) {
                    char *value_begin = equals + 1, *value_end = end;
                    if (trim(&value_begin, &value_end)) {
                        process_attribute(channel, key_begin, key_end, value_begin, value_end);
                        return;
                    }
                    else {
                        zigbee_send(channel, "ERROR excepted option value");
                    }
                }
                else {
                    zigbee_send(channel, "ERROR excepted option name");
                }
            }
            else {
                char *command_begin = begin, *command_end = end;
                if (trim(&command_begin, &command_end)) {
                    process_command(channel, command_begin, command_end);
                }
                else {
                    zigbee_send(channel, "ERROR excepted command");
                }
            }
        }
        else {
            zigbee_send(0, "ERROR invalid sequence number");
        }
    }
    else {
        zigbee_send(0, "ERROR excepted channel");
    }
}
