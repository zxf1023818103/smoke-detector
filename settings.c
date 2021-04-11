/*
 * settings.c
 *
 *  Created on: 2021/2/25
 *      Author: Zeng Xiangfei
 */

#include <string.h>
#include <msp430.h>

#ifdef __TI_COMPILER_VERSION__

#pragma PERSISTENT(sensitivity)
static unsigned int sensitivity = 900;

#pragma PERSISTENT(alarm_silent_cycles)
static unsigned int alarm_silent_cycles = 96;

#pragma PERSISTENT(adc_value_report_enabled)
static char adc_value_report_enabled = 0;

#pragma PERSISTENT(alarm_report_channel)
static unsigned int alarm_report_channel = 0;

#pragma PERSISTENT(adc_value_report_channel)
static unsigned int adc_value_report_channel = 0;

#pragma PERSISTENT(name)
static char name[32] = "Smoke Detector";

#pragma PERSISTENT(id)
static unsigned int id = 0;

#elif __IAR_SYSTEMS_ICC__

__persistent static unsigned int sensitivity = 700;
__persistent static unsigned int alarm_silent_cycles = 96;
__persistent static char adc_value_report_enabled = 0;
__persistent static unsigned int alarm_report_channel = 0;
__persistent static unsigned int adc_value_report_channel = 0;
__persistent static char name[32] = "Smoker Detector";
__persistent static unsigned int id = 0;

#else

static unsigned int sensitivity = 700;
static unsigned int alarm_silent_cycles = 96;
static char adc_value_report_enabled = 0;
static unsigned int alarm_report_channel = 0;
static unsigned int adc_value_report_channel = 0;
static char name[32] = "Smoker Detector";
static unsigned int id = 0;

#endif

void settings_set_sensitivity(unsigned int value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    sensitivity = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

unsigned int settings_get_sensitivity() {
    return sensitivity;
}

void settings_set_alarm_silent_cycles(unsigned int value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    alarm_silent_cycles = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

unsigned int settings_get_alarm_silent_cycles() {
    return alarm_silent_cycles;
}

void settings_set_alarm_report_channel(unsigned int value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    alarm_report_channel = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

unsigned int settings_get_alarm_report_channel() {
    return alarm_report_channel;
}

void settings_set_adc_value_report_channel(unsigned int value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    adc_value_report_channel = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

unsigned int settings_get_adc_value_report_channel() {
    return adc_value_report_channel;
}

void settings_set_adc_value_report_enabled(char value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    adc_value_report_enabled = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

char settings_get_adc_value_report_enabled() {
    return adc_value_report_enabled;
}

void settings_set_name(char *value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    strncpy(name, value, sizeof name);
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

const char *settings_get_name() {
    return name;
}

void settings_set_id(unsigned int value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    id = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

unsigned int settings_get_id() {
    return id;
}
