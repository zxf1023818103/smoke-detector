/*
 * zigbee.c
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include <string.h>
#include "smokedetector.h"

void zigbee_send(unsigned int sequence_number, const char *str) {
    char buffer[7];
    uart_print(my_itoa(sequence_number, buffer));
    uart_print(" ");
    uart_puts(str);
}

void zigbee_println(unsigned int sequence_number, unsigned short value) {
    char str[7];
    zigbee_send(sequence_number, my_itoa(value, str));
}

int isblank(char c) {
    return c == ' ' || c == '\t';
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

static int isdigit(char c) {
    return '0' <= c && c <= '9';
}

static void process_option(unsigned int sequence_number, char *key_begin, char *key_end, char *value_begin, char *value_end) {
    *key_end = 0;
    *value_end = 0;
    if (*value_begin == '?') {
        const char *value = attribute_read_callback(key_begin);
        if (value) {
            zigbee_send(sequence_number, value);
        }
        else {
            zigbee_send(sequence_number, "ERROR invalid key");
        }
    }
    else {
        int result = attribute_write_callback(key_begin, value_begin);
        switch (result) {
        case 0:
            zigbee_send(sequence_number, "OK");
            break;
        case 1:
            zigbee_send(sequence_number, "ERROR invalid key or unsupported operation");
            break;
        case 2:
            zigbee_send(sequence_number, "ERROR invalid value");
            break;
        default:
            break;
        }
    }
}

static void process_command(unsigned int sequence_number, char *command_begin, char *command_end) {
    *command_end = 0;
    command_received_callback(command_begin);
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

    char *blank = strchr(begin, ' ');
    if (blank) {
        char *sequence_number_begin = begin, *sequence_number_end = begin;
        while (isdigit(*sequence_number_end)) {
            sequence_number_end++;
        }
        if (sequence_number_end == blank) {
            *sequence_number_end = 0;

            unsigned int sequence_number;
            my_atoi(sequence_number_begin, &sequence_number);

            char *equals = strchr(blank + 1, '=');
            if (equals) {
                char *key_begin = blank + 1, *key_end = equals;
                if (trim(&key_begin, &key_end)) {
                    char *value_begin = equals + 1, *value_end = end;
                    if (trim(&value_begin, &value_end)) {
                        process_option(sequence_number, key_begin, key_end, value_begin, value_end);
                        return;
                    }
                    else {
                        zigbee_send(sequence_number, "ERROR excepted option value");
                    }
                }
                else {
                    zigbee_send(sequence_number, "ERROR excepted option name");
                }
            }
            else {
                char *command_begin = begin, *command_end = end;
                if (trim(&command_begin, &command_end)) {
                    process_command(sequence_number, command_begin, command_end);
                }
                else {
                    zigbee_send(sequence_number, "ERROR excepted command");
                }
            }
        }
        else {
            zigbee_send(0, "ERROR invalid sequence number");
        }
    }
    else {
        zigbee_send(0, "ERROR excepted sequence number");
    }
}
