cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_TOOLCHAIN_FILE ./msp430-elf-gcc.cmake)

project(smoke-detector C)

add_executable(${CMAKE_PROJECT_NAME}.elf adc.c alarm.c button.c buzzer.c clock.c led.c main.c settings.c smokedetector.h timer.c uart.c watchdog.c)

set(DEVICE MSP430FR2311)
set(MAP smoke-detector.map)

target_link_options(${CMAKE_PROJECT_NAME}.elf PRIVATE -mmcu=${DEVICE} -Os -Wall -Wl,-Map,${MAP},--gc-sections -s)
target_compile_options(${CMAKE_PROJECT_NAME}.elf PRIVATE -mmcu=${DEVICE} -Os -Wall -c)
add_custom_target(${CMAKE_PROJECT_NAME}.out
                DEPENDS ${CMAKE_PROJECT_NAME}.elf
                COMMAND ${CMAKE_OBJCOPY} -Oihex -S ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.out)
add_custom_target(${CMAKE_PROJECT_NAME}.bin
                DEPENDS ${CMAKE_PROJECT_NAME}.elf
                COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin)
