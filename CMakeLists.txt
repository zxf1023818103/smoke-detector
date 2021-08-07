cmake_minimum_required(VERSION 3.2.0)

set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)


project(smoke-detector C)

add_executable(smoke-detector.out adc.c alarm.c button.c buzzer.c clock.c led.c main.c settings.c smokedetector.h timer.c uart.c watchdog.c)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(TOOLCHAIN_DIR /opt/ti/msp430-gcc)
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR})

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/msp430-elf-gcc)
set(DEVICE MSP430FR2311)
set(MAP smoke-detector.map)


target_include_directories(smoke-detector.out PRIVATE /opt/ti/msp430-gcc/include)
target_link_directories(smoke-detector.out PRIVATE /opt/ti/msp430-gcc/include)
target_link_options(smoke-detector.out PRIVATE -mmcu=${DEVICE} -Os -Wall -Wl,-Map,${MAP},--gc-sections -s)
target_compile_options(smoke-detector.out PRIVATE -mmcu=${DEVICE} -Os -Wall -c)
