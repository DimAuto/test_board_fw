cmake_minimum_required(VERSION 3.1...3.24)

add_subdirectory(core)

set(srcs ./battery.c ./encoder.c ./gpio_peripherals.c ./internal_uart.c ./max5418.c ./message_handler.c ./test_board_leds.c)
add_library(src STATIC ${srcs})
target_include_directories(src PUBLIC . .. "${TIDIR}/${COMPILER_NAME}/include"
"../includes")
target_link_libraries(src core)