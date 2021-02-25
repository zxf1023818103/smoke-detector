/*
 * settings.c
 *
 *  Created on: 2021/2/25
 *      Author: Zeng Xiangfei
 */

#include <msp430.h>

#ifdef __TI_COMPILER_VERSION__

#pragma PERSISTENT(sensitivity)
unsigned int sensitivity = 700;

#pragma PERSISTENT(alarm_silent_cycles)
unsigned int alarm_silent_cycles = 96;

#pragma PERSISTENT(adc_output_enabled)
char adc_output_enabled = 0;

#elif __IAR_SYSTEMS_ICC__

__persistent unsigned int sensitivity = 700;
__persistent unsigned int alarm_silent_cycles = 96;
__persistent char adc_output_enabled = 0;

#else

unsigned int sensitivity = 700;
unsigned int alarm_silent_cycles = 96;
char adc_output_enabled = 0;

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

void settings_set_adc_output_enabled(char value) {
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable
    adc_output_enabled = value;
    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}

char settings_get_adc_output_enabled() {
    return adc_output_enabled;
}
