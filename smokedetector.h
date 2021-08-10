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
unsigned int alarm_get_remain_silent_cycles();
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

void zigbee_send(unsigned int channel, const char *str);
void zigbee_report_attribute(unsigned int channel, const char *attribute, unsigned short value);
void zigbee_send_read_request_response(unsigned int channel, const char *str);

void settings_set_sensitivity(unsigned int value);
unsigned int settings_get_sensitivity();
void settings_set_alarm_silent_cycles(unsigned int value);
unsigned int settings_get_alarm_silent_cycles();
void settings_set_alarm_report_channel(unsigned int value);
unsigned int settings_get_alarm_report_channel();
void settings_set_adc_value_report_channel(unsigned int value);
unsigned int settings_get_adc_value_report_channel();
void settings_set_adc_value_report_enabled(char value);
char settings_get_adc_value_report_enabled();
void settings_set_name(char *value);
const char *settings_get_name();
void settings_set_id(unsigned int value);
unsigned int settings_get_id();

void command_received_callback(unsigned int channel, char *command) __attribute__((weak));
const char* attribute_read_callback(unsigned int channel, char *attribute) __attribute__((weak));
int attribute_write_callback(unsigned int channel, char *attribute, char *value) __attribute__((weak));

int is_digit(char c);
int isblank(int c);
int my_atoi(char *value, unsigned int *result);
char* my_itoa(unsigned int value, char *buffer);
char* reverse(char *str, int size);
int my_strcmp(const char *str1, const char *str2);
char *my_strchr(const char *str, char c);
void my_strncpy(char *dst, char *src, unsigned int size);

#endif /* SMOKEDETECTOR_H_ */
