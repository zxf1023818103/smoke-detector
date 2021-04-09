/*
 * main.c
 *
 *  Created on: 2021/2/24
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>
#include "smokedetector.h"

const char *id_string = "id";
const char *name_string = "name";
const char *test_string = "test";
const char *sensitiviy_string = "sensitivity";
const char *alarm_string = "alarm";
const char *adc_value_string = "adc_value";
const char *adc_value_report_enabled_string = "adc_value_report_enabled";
const char *alarm_silent_cycles_string = "alarm_silent_cycles";
const char *remain_alarm_silent_cycles_string = "remain_alarm_silent_cycles";

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

static unsigned short adc_value;

void adc_sample_callback(unsigned short value) {
    adc_value = value;
    if (settings_get_adc_value_report_enabled()) {
        zigbee_report_attribute(settings_get_adc_value_report_channel(), adc_value_string, value);
    }
    if (value > settings_get_sensitivity()) {
        alarm_on();
    }
}

void timer_500ms_callback() {
    alarm_loop();
    adc_sample();
}

const char* attribute_read_callback(unsigned int channel, char *attribute) {
    static char result[6];
    if (my_strcmp(id_string, attribute) == 0) {
        return my_itoa(settings_get_id(), result);
    }
    else if (my_strcmp(name_string, attribute) == 0) {
        return settings_get_name();
    }
    else if (my_strcmp(test_string, attribute) == 0) {
        return my_itoa(alarm_is_test(), result);
    }
    else if (my_strcmp(sensitiviy_string, attribute) == 0) {
        return my_itoa(settings_get_sensitivity(), result);
    }
    else if (my_strcmp(alarm_string, attribute) == 0) {
        settings_set_alarm_report_channel(channel);
        return my_itoa(alarm_status(), result);
    }
    else if (my_strcmp(adc_value_string, attribute) == 0) {
        return my_itoa(adc_value, result);
    }
    else if (my_strcmp(adc_value_report_enabled_string, attribute) == 0) {
        return my_itoa(settings_get_adc_value_report_enabled(), result);
    }
    else if (my_strcmp(remain_alarm_silent_cycles_string, attribute) == 0) {
        return my_itoa(alarm_get_remain_silent_cycles(), result);
    }
    else if (my_strcmp(alarm_silent_cycles_string, attribute) == 0) {
        return my_itoa(settings_get_alarm_silent_cycles(), result);
    }
    return 0;
}

int attribute_write_callback(unsigned int channel, char *attribute, char *value) {

    unsigned int int_value;
    int result = my_atoi(value, &int_value);

    if (my_strcmp(name_string, attribute) == 0) {
        settings_set_name(value);
        return 0;
    }
    else if (result) {
        if (my_strcmp(id_string, attribute) == 0) {
            settings_set_id(int_value);
        }
        else if (my_strcmp(test_string, attribute) == 0) {
            if (int_value) {
                alarm_test();
            }
        }
        else if (my_strcmp(sensitiviy_string, attribute) == 0) {
            settings_set_sensitivity(int_value);
        }
        else if (my_strcmp(alarm_string, attribute) == 0) {
            settings_set_alarm_report_channel(channel);
            if (int_value) {
                alarm_on();
            }
            else {
                alarm_silent();
            }
        }
        else if (my_strcmp(adc_value_report_enabled_string, attribute) == 0) {
            settings_set_adc_value_report_enabled(int_value != 0);
            settings_set_adc_value_report_channel(channel);
        }
        else if (my_strcmp(alarm_silent_cycles_string, attribute) == 0) {
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

void command_received_callback(unsigned int channel, char *command) {

}
