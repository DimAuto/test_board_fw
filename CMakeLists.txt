cmake_minimum_required(VERSION 3.1...3.24)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(PROJECT_NAME "bnvd-test_board-fw")

set(EXECUTABLE_NAME bnvd-test-board-fw)

set(COMPILER_NAME "ti-cgt-msp430_21.6.0.LTS")

if (WIN32)
    message("OS Type: Windows")
    set(TIDIR "C:/ti/ccs1120/ccs/tools/compiler")
    set(COMP_EXECUTABLE "cl430.exe")
    set(HEX_COMP_EXECUTABLE "hex430.exe")
elseif (UNIX)
    message("OS type: LINUX")
    set(TIDIR "/opt/ti")
    set(COMP_EXECUTABLE "cl430")
    set(HEX_COMP_EXECUTABLE "hex430")
endif()

set(MSP430_MCU msp430f6638)

set(CMAKE_C_COMPILER "${TIDIR}/${COMPILER_NAME}/bin/${COMP_EXECUTABLE}")
set(EXECUTABLE_OUTPUT_PATH ..)
add_compile_definitions(__MSP430F6638__)
add_compile_definitions(_DEBUG_)

add_compile_options(-vmspx -O0 --opt_for_speed=3 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40)
add_link_options(--heap_size=160 --stack_size=160 --rom_model --library=../lnk_msp430f6638.cmd --output_file=${EXECUTABLE_NAME}.out)

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C) 

add_subdirectory(src)

add_executable(main ./src/main.c)
target_include_directories(main PUBLIC . "./includes" "${TIDIR}/${COMPILER_NAME}/lib")

add_custom_command(
    TARGET main POST_BUILD
    COMMAND "${TIDIR}/${COMPILER_NAME}/bin/${HEX_COMP_EXECUTABLE}"
        --memwidth=8
        --romwidth=8
        --diag_wrap=off
        --ti_txt
        -o "./bnvd-test-board-fw.txt" "./bnvd-test-board-fw.out"
    DEPENDS "./bnvd-test-board-fw.out"
    )
  

target_link_libraries(main src)