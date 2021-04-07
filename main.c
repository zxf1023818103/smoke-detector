/*
 * main.c
 *
 *  Created on: 2021/2/24
 *      Author: Zeng Xiangfei
 */

#include <stdio.h>
#include <string.h>
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

void adc_sample_callback(unsigned short adc_value) {
    if (settings_get_adc_value_report_enabled()) {
        zigbee_println(settings_get_adc_value_report_channel(), adc_value);
    }
    if (adc_value > settings_get_sensitivity()) {
        alarm_on();
    }
}

void timer_500ms_callback() {
    alarm_loop();
    adc_sample();
}

static const char *id_string = "id";
static const char *name_string = "name";
static const char *test_string = "test";
static const char *sensitiviy_string = "sensitivity";
static const char *alarm_string = "alarm";
static const char *adc_value_report_enabled_string = "adc_value_report_enabled";
static const char *alarm_silent_cycles_string = "alarm_silent_cycles";
static const char *remain_alarm_silent_cycles_string = "remain_alarm_silent_cycles";

static char* reverse(char *str, int size) {
    char *i = str, *j = str + size - 1;
    while (i < j) {
        char t = *i;
        *i = *j;
        *j = t;
        i++; j--;
    }
    return str;
}

char* my_itoa(unsigned int value, char *buffer) {
    unsigned int i = 0;
    for (;;) {
        buffer[i++] = value % 10 + '0';
        value /= 10;
        if (value == 0) {
            break;
        }
    }
    buffer[i] = 0;
    return reverse(buffer, i);
}

const char* attribute_read_callback(unsigned int sequence_number, char *attribute) {
    static char result[6];
    if (strcmp(id_string, attribute) == 0) {
        return my_itoa(settings_get_id(), result);
    }
    else if (strcmp(name_string, attribute) == 0) {
        return settings_get_name();
    }
    else if (strcmp(test_string, attribute) == 0) {
        return my_itoa(alarm_is_test(), result);
    }
    else if (strcmp(sensitiviy_string, attribute) == 0) {
        return my_itoa(settings_get_sensitivity(), result);
    }
    else if (strcmp(alarm_string, attribute) == 0) {
        settings_set_alarm_report_channel(sequence_number);
        return my_itoa(alarm_status(), result);
    }
    else if (strcmp(adc_value_report_enabled_string, attribute) == 0) {
        return my_itoa(settings_get_adc_value_report_enabled(), result);
    }
    else if (strcmp(remain_alarm_silent_cycles_string, attribute) == 0) {
        return my_itoa(alarm_get_remain_silent_cycles(), result);
    }
    else if (strcmp(alarm_silent_cycles_string, attribute) == 0) {
        return my_itoa(settings_get_alarm_silent_cycles(), result);
    }
    return 0;
}

int is_digit(char c) {
    return '0' <= c && c <= '9';
}

int my_atoi(char *value, unsigned int *result) {
    char *end = value;
    while (*end) {
        end++;
    }
    end--;
    int digit = 1;
    *result = 0;
    while (value <= end) {
        if (is_digit(*end)) {
            *result += digit * (*end - '0');
            digit *= 10;
            end--;
        }
        else {
            return 0;
        }
    }
    return 1;
}

int attribute_write_callback(unsigned int sequence_number, char *attribute, char *value) {

    unsigned int int_value;
    int result = my_atoi(value, &int_value);

    if (strcmp(name_string, attribute) == 0) {
        settings_set_name(value);
    }
    else if (result) {
        if (strcmp(id_string, attribute) == 0) {
            settings_set_id(int_value);
        }
        else if (strcmp(test_string, attribute) == 0) {
            if (int_value) {
                alarm_test();
            }
        }
        else if (strcmp(sensitiviy_string, attribute) == 0) {
            settings_set_sensitivity(int_value);
        }
        else if (strcmp(alarm_string, attribute) == 0) {
            settings_set_alarm_report_channel(sequence_number);
            if (int_value) {
                alarm_on();
            }
            else {
                alarm_silent();
            }
        }
        else if (strcmp(adc_value_report_enabled_string, attribute) == 0) {
            settings_set_adc_value_report_enabled(int_value != 0);
            settings_set_adc_value_report_channel(sequence_number);
        }
        else if (strcmp(alarm_silent_cycles_string, attribute) == 0) {
            settings_set_alarm_silent_cycles(int_value);
        }
        else {
            /// unsupported operation or invalid key
            return 1;
        }
        /// ok
        return 0;
    }
    /// invalid value
    return 2;
}

void command_received_callback(unsigned int sequence_number, char *command) {

}
