#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.17)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(xm_rc C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")
set(COMMON_FLAGS "-O0 -g -ggdb -Wall -fno-strict-aliasing -ffunction-sections -fdata-sections -fno-exceptions -fno-delete-null-pointer-checks -fmessage-length=0 -fno-builtin -mthumb")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -fno-rtti -std=c++11")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -std=c99")

add_link_options(-specs=nano.specs
                 -Wl,--gc-sections
                 -u _printf_float
)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(Inc 
                    Drivers/STM32F1xx_HAL_Driver/Inc 
                    Drivers/STM32F1xx_HAL_Driver/Inc/Legacy 
                    Drivers/CMSIS/Device/ST/STM32F1xx/Include 
                    Drivers/CMSIS/Include 
                    Drivers/SSD1306/Inc
                    Libraries/AP_Buffer
                    Libraries/AP_Math
                    Libraries/AP_Show
                    Libraries/mavlink
                    Libraries/mavlink/car
                    Libraries/RC_Channel
                    Libraries/rt-thread
                    Libraries/rt-thread/board-f103c8
                    Libraries/rt-thread/components
                    Libraries/rt-thread/components/cplusplus
                    Libraries/rt-thread/components/drivers
                    Libraries/rt-thread/components/drivers/include
                    Libraries/rt-thread/components/drivers/include/drivers/
                    Libraries/rt-thread/components/drivers/include/ipc
                    Libraries/rt-thread/components/finsh
                    Libraries/rt-thread/components/libc
                    Libraries/rt-thread/components/libc/compilers
                    Libraries/rt-thread/components/libc/compilers/common
                    Libraries/rt-thread/components/libc/compilers/common/sys
                    Libraries/rt-thread/HAL_Drivers
                    Libraries/rt-thread/HAL_Drivers/config
                    Libraries/rt-thread/HAL_Drivers/config/f1
                    Libraries/rt-thread/include
                    Libraries/rt-thread/include/libc
                    packages/nrf24l01-latest/src
                    packages/FlexibleButton-latest
)

add_definitions(-DUSE_HAL_DRIVER -DSTM32F103xB -DUSE_RTTHREAD)

file(GLOB_RECURSE SOURCES
     "IDE/GCC/*.*"
     "Drivers/CMSIS/*.*"
     "Drivers/SSD1306/*.*"
     "Drivers/STM32F1xx_HAL_Driver/*.*"
     "Src-xm/*.*"
     "Libraries/AP_Buffer/*.*"
     "Libraries/AP_Math/*.*"
     "Libraries/AP_Show/*.*"
     "Libraries/RC_Channel/*.*"
     "Libraries/rt-thread/src/*.*"
     "Libraries/rt-thread/board-f103c8/*.*"
     "Libraries/rt-thread/libc/*.*"
     "packages/nrf24l01-latest/src/*.*"
)

list(APPEND SOURCES
     IDE/GCC/startup_stm32f103xb-rtt.S;
     Libraries/rt-thread/libcpu/arm/cortex-m3/context_gcc.S
     Libraries/rt-thread/components/cplusplus/cxx_Mutex.cpp;
     Libraries/rt-thread/components/cplusplus/cxx_Semaphore.cpp;
     Libraries/rt-thread/components/cplusplus/cxx_Thread.cpp;
     Libraries/rt-thread/components/cplusplus/cxx_crt.cpp;
     Libraries/rt-thread/components/cplusplus/crt_init.c;
     Libraries/rt-thread/components/drivers/misc/adc.c;
     Libraries/rt-thread/components/drivers/misc/pin.c;
     Libraries/rt-thread/components/drivers/serial/serial.c;
     Libraries/rt-thread/components/drivers/spi/spi_core.c;
     Libraries/rt-thread/components/drivers/spi/spi_dev.c;
     Libraries/rt-thread/components/drivers/src/completion.c;
     Libraries/rt-thread/components/drivers/src/dataqueue.c;
     Libraries/rt-thread/components/drivers/src/ringbuffer.c;
     Libraries/rt-thread/components/finsh/cmd.c;
     Libraries/rt-thread/components/finsh/msh.c;
     Libraries/rt-thread/components/finsh/msh_file.c;
     Libraries/rt-thread/components/finsh/shell.c;
     Libraries/rt-thread/components/finsh/symbol.c;
     Libraries/rt-thread/HAL_Drivers/drv_adc.c;
     Libraries/rt-thread/HAL_Drivers/drv_common.c;
     Libraries/rt-thread/HAL_Drivers/drv_gpio.c;
     Libraries/rt-thread/HAL_Drivers/drv_spi.c;
     Libraries/rt-thread/HAL_Drivers/drv_usart.c;
     Libraries/rt-thread/libcpu/arm/cortex-m3/cpuport.c
     packages/FlexibleButton-latest/flexible_button.c;
     # GNU RT_USING_NEWLIB
     Libraries/rt-thread/components/libc/compilers/newlib/libc_syms.c;
     Libraries/rt-thread/components/libc/compilers/newlib/stdio.c;
     Libraries/rt-thread/components/libc/compilers/newlib/syscalls.c;
     Libraries/rt-thread/components/libc/compilers/newlib/time.c;
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/IDE/GCC/link.lds)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
