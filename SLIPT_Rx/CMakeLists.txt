cmake_minimum_required(VERSION 3.20)

# Override the default system to cross-compile for ARM processors
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)

project(SLIPT_Rx C CXX ASM)

# Disable automatic macOS -arch flags
set(CMAKE_OSX_ARCHITECTURES "")

# Force ARM GCC
set(CMAKE_C_COMPILER   /Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER /Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin/arm-none-eabi-gcc)

# MCU flags
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -O2 -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -Wall -Wextra")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -T${CMAKE_SOURCE_DIR}/AmbiqSuite/boards/apollo3p_evb/examples/prime/gcc/linker_script.ld -Wl,--gc-sections")

# AMBIQ SDK path (set this before configuring if not already set)
if(NOT DEFINED AMBIQ_SDK_PATH)
    set(AMBIQ_SDK_PATH "${CMAKE_SOURCE_DIR}/AmbiqSuite")
endif()

# Include directories
include_directories(
    ${AMBIQ_SDK_PATH}/mcu/apollo3
    ${AMBIQ_SDK_PATH}/boards/apollo3p_evb/bsp
    ${AMBIQ_SDK_PATH}/CMSIS/AmbiqMicro/Include
    ${AMBIQ_SDK_PATH}/CMSIS/ARM/Include
    ${AMBIQ_SDK_PATH}/devices
    ${AMBIQ_SDK_PATH}/utils
    ${CMAKE_SOURCE_DIR}/components/S2
)

# Source files
file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/main/*.c
    ${CMAKE_SOURCE_DIR}/main/*.cpp
    ${AMBIQ_SDK_PATH}/mcu/apollo3/hal/startup/startup_apollo3.c
)

# Executable
add_executable(${PROJECT_NAME}.elf ${SRC_FILES})

# Post-build: generate binary and size info
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND /Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin/arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND /Applications/ArmGNUToolchain/14.3.rel1/arm-none-eabi/bin/arm-none-eabi-size ${PROJECT_NAME}.elf
)
