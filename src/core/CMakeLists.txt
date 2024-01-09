cmake_minimum_required(VERSION 3.1...3.24)

set(cores ./adc.c ./gpio.c ./helpers.c ./i2c.c ./led.c ./ring_buffer.c ./timer.c ./uart.c)
add_library(core STATIC ${cores})
target_include_directories(core PUBLIC .. . ../.. "${TIDIR}/${COMPILER_NAME}/include"
"../../includes")
target_link_libraries(core "${TIDIR}/${COMPILER_NAME}/lib/libc.a")