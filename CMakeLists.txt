cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_TOOLCHAIN_FILE ./msp430-elf-gcc.cmake)

project(smoke-detector C)

add_executable(smoke-detector.out adc.c alarm.c button.c buzzer.c clock.c led.c main.c settings.c smokedetector.h timer.c uart.c watchdog.c)

set(DEVICE MSP430FR2311)
set(MAP smoke-detector.map)

target_link_options(smoke-detector.out PRIVATE -mmcu=${DEVICE} -Os -Wall -Wl,-Map,${MAP},--gc-sections -s)
target_compile_options(smoke-detector.out PRIVATE -mmcu=${DEVICE} -Os -Wall -c)
