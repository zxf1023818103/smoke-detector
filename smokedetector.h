/*
 * smokedetector.h
 *
 *  Created on: 2021/2/21
 *      Author: Zeng Xiangfei
 */

#ifndef SMOKEDETECTOR_H_
#define SMOKEDETECTOR_H_

void software_trim();
void clock_1mhz_init();

void led_init();
void led_on();
void led_off();
void led_toggle();

void ir_init();
void ir_on();
void ir_off();

void buzzer_init();
void buzzer_on();
void buzzer_off();
int buzzer_status();

void alarm_on();
void alarm_off();
void alarm_silent();
int alarm_status();
void alarm_test();
char alarm_is_test();
void alarm_loop();

void adc_init();
void adc_sample();
void adc_sample_callback(unsigned short adc_value) __attribute__((weak));

void button_init();
void button_pressed_callback() __attribute__((weak));
void button_released_callback() __attribute__((weak));

void timer_500ms_init();
void timer_500ms_callback() __attribute__((weak));

void watchdog_init();

void uart_38400bps_init();
void uart_putchar(char c);
void uart_print(const char *str);
void uart_puts(const char *str);
void uart_write(const char *data, unsigned int len);
void uart_newline_callback(char *str, unsigned int len) __attribute__((weak));

void zigbee_send(const char *str);
void zigbee_println(unsigned short value);

void settings_set_sensitivity(unsigned int value);
unsigned int settings_get_sensitivity();
void settings_set_alarm_silent_cycles(unsigned int value);
unsigned int settings_get_alarm_silent_cycles();
void settings_set_adc_output_enabled(char value);
char settings_get_adc_output_enabled();
void settings_set_name(char *value);
const char *settings_get_name();

void command_received_callback(char *command) __attribute__((weak));
const char* attribute_read_callback(char *attribute) __attribute__((weak));
void attribute_write_callback(char *attribute, char *value) __attribute__((weak));

#endif /* SMOKEDETECTOR_H_ */
