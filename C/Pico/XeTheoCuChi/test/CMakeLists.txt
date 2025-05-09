cmake_minimum_required(VERSION 3.15)

# Set project name
set(PROJECT_NAME test)

# Determine board from pico-sdk
# (choose one board from ${PICO_SDK_PATH}/src/boards/include/boards/)
set(PICO_BOARD pico)

# Set C, C++ standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

# Import pico-sdk
include(pico_sdk_import.cmake)

# Initiate pico-sdk -> library functions
pico_sdk_init()

# Determine project with CMake using both C, C++
project(${PROJECT_NAME} C CXX)

# Set target name
set(TARGET_NAME ${PROJECT_NAME})

# Build executable file (.uf2) from car.c
add_executable(${TARGET_NAME}
    car.c
)

# Macro definitions (empty for now)
target_compile_definitions(${TARGET_NAME} PRIVATE)

# Include directories (empty for now)
target_include_directories(${TARGET_NAME} PRIVATE)

# Link necessary libraries
target_link_libraries(${TARGET_NAME} PRIVATE
    pico_stdlib
    hardware_gpio
    hardware_uart
    hardware_irq
    hardware_pwm
)

# Generate extra build files: .bin, .uf2, .hex
pico_add_extra_outputs(${TARGET_NAME})

# Enable UART for printf(), scanf()
pico_enable_stdio_uart(${TARGET_NAME} 1)

# Uncomment to enable USB CDC (console over USB)
# pico_enable_stdio_usb(${TARGET_NAME} 1)
