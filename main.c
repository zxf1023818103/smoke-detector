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
    if (settings_get_adc_output_enabled()) {
        zigbee_println(adc_value);
    }
    if (adc_value > settings_get_sensitivity()) {
        alarm_on();
    }
}

void timer_500ms_callback() {
    alarm_loop();
    adc_sample();
}

static const char *name_string = "name";
static const char *test_string = "test";
static const char *sensitiviy_string = "sensitivity";
static const char *alarm_string = "alarm";
static const char *adc_output_enabled_string = "adc_output_enabled";
static const char *alarm_silent_cycles_string = "alarm_silent_cycles";
static const char *ok_string = "ok";

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

static char* my_itoa(unsigned int value, char *buffer) {
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

const char* attribute_read_callback(char *attribute) {
    static char result[6];
    if (strcmp(name_string, attribute) == 0) {
        return settings_get_name();
    }
    else if (strcmp(test_string, attribute) == 0) {
        return my_itoa(alarm_is_test(), result);
    }
    else if (strcmp(sensitiviy_string, attribute) == 0) {
        return my_itoa(settings_get_sensitivity(), result);
    }
    else if (strcmp(alarm_string, attribute) == 0) {
        return my_itoa(alarm_status(), result);
    }
    else if (strcmp(adc_output_enabled_string, attribute) == 0) {
        return my_itoa(settings_get_adc_output_enabled(), result);
    }
    else if (strcmp(alarm_silent_cycles_string, attribute) == 0) {
        return my_itoa(settings_get_alarm_silent_cycles(), result);
    }
    return 0;
}

static int is_digit(char c) {
    return '0' <= c && c <= '9';
}

static int my_atoi(char *value, unsigned int *result) {
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

void attribute_write_callback(char *attribute, char *value) {

    unsigned int int_value;
    int result = my_atoi(value, &int_value);

    if (strcmp(name_string, attribute) == 0) {
        settings_set_name(value);
        zigbee_send(ok_string);
    }
    else if (result) {
        if (strcmp(test_string, attribute) == 0) {
            if (int_value) {
                alarm_test();
                zigbee_send(ok_string);
            }
        }
        else if (strcmp(sensitiviy_string, attribute) == 0) {
            settings_set_sensitivity(int_value);
            zigbee_send(ok_string);
        }
        else if (strcmp(alarm_string, attribute) == 0) {
            if (int_value) {
                alarm_on();
            }
            else {
                alarm_silent();
            }
            zigbee_send(ok_string);
        }
        else if (strcmp(adc_output_enabled_string, attribute) == 0) {
            settings_set_adc_output_enabled(int_value != 0);
            zigbee_send(ok_string);
        }
        else if (strcmp(alarm_silent_cycles_string, attribute) == 0) {
            settings_set_alarm_silent_cycles(int_value);
            zigbee_send(ok_string);
        }
    }
}

void command_received_callback(char *command) {

}
