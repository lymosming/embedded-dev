cmake_minimum_required(VERSION 3.20)

include(cmake/arm-gcc-toolchain.cmake)

# ========================
# Project
# ========================
project(myproject C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ========================
# MCU / Compiler flags
# ========================
set(MCU_FLAGS
    -mcpu=cortex-m3
    -mthumb
)

set(COMMON_FLAGS
    ${MCU_FLAGS}
    -ffunction-sections
    -fdata-sections
    -Wall
    -g
    -O0
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(${COMMON_FLAGS})

# ========================
# Linker flags
# ========================
add_link_options(
    ${MCU_FLAGS}
    -Wl,--gc-sections
    --specs=nosys.specs
    --specs=nano.specs
)

# ========================
# Compile definitions
# (same as Embedded IDE)
# ========================
add_compile_definitions(
    USE_STDPERIPH_DRIVER
    STM32F10X_MD
)

# ========================
# Include directories
# ========================
include_directories(
    src
    src/Hardware
    src/System
    lib/cmsis
    hal/STM32F10x_StdPeriph_Driver
    hal/STM32F10x_StdPeriph_Driver/inc
)

# ========================
# Source files
# (include EVERYTHING in StdPeriph Driver)
# ========================
file(GLOB_RECURSE PROJECT_SOURCES
    src/*.c
    src/Hardware/*.c
    src/System/*.c
    hal/STM32F10x_StdPeriph_Driver/*.c
)

# Startup assembly
set(STARTUP_FILE
    src/startup_stm32f10x_md.s
)

# ========================
# Target
# ========================
add_executable(${PROJECT_NAME}.elf
    ${PROJECT_SOURCES}
    ${STARTUP_FILE}
)

# ========================
# Linker script
# ========================
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/stm32f1x_64KB_flash.ld
)

# Math library
target_link_libraries(${PROJECT_NAME}.elf
    m
)

# ========================
# Output files (.bin / .hex)
# ========================
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex
            ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)

# ========================
# ST-Link flash
# ========================
add_custom_target(flash
    COMMAND st-flash write
            ${PROJECT_NAME}.bin 0x08000000
    DEPENDS ${PROJECT_NAME}.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing firmware to STM32 via ST-Link"
)

