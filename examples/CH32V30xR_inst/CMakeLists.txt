cmake_minimum_required(VERSION 3.22)

include("cmake/toolchain.cmake")

project(CH32V30xR_example VERSION 0.0.1 LANGUAGES C CXX ASM)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    SRC/Startup/startup_ch32v30x_D8.S
    SRC/Peripheral/src/ch32v30x_adc.c
    SRC/Peripheral/src/ch32v30x_bkp.c
    SRC/Peripheral/src/ch32v30x_can.c
    SRC/Peripheral/src/ch32v30x_crc.c
    SRC/Peripheral/src/ch32v30x_dac.c
    SRC/Peripheral/src/ch32v30x_dbgmcu.c
    SRC/Peripheral/src/ch32v30x_dma.c
    SRC/Peripheral/src/ch32v30x_dvp.c
    SRC/Peripheral/src/ch32v30x_eth.c
    SRC/Peripheral/src/ch32v30x_exti.c
    SRC/Peripheral/src/ch32v30x_flash.c
    SRC/Peripheral/src/ch32v30x_fsmc.c
    SRC/Peripheral/src/ch32v30x_gpio.c
    SRC/Peripheral/src/ch32v30x_i2c.c
    SRC/Peripheral/src/ch32v30x_iwdg.c
    SRC/Peripheral/src/ch32v30x_misc.c
    SRC/Peripheral/src/ch32v30x_opa.c
    SRC/Peripheral/src/ch32v30x_pwr.c
    SRC/Peripheral/src/ch32v30x_rcc.c
    SRC/Peripheral/src/ch32v30x_rng.c
    SRC/Peripheral/src/ch32v30x_rtc.c
    SRC/Peripheral/src/ch32v30x_sdio.c
    SRC/Peripheral/src/ch32v30x_spi.c
    SRC/Peripheral/src/ch32v30x_tim.c
    SRC/Peripheral/src/ch32v30x_usart.c
    SRC/Peripheral/src/ch32v30x_wwdg.c

#    SRC/Debug/debug.c
    User/debug.c

    User/ch32v30x_it.c
    User/main.c
    User/system_ch32v30x.c

    trice/trice.c
    trice/cobsDecode.c
    trice/cobsEncode.c
    trice/tcobsv1Decode.c
    trice/tcobsv1Encode.c
    trice/trice.c
    trice/trice8.c
    trice/trice16.c
    trice/trice32.c
    trice/trice64.c
    trice/triceUart.c
    trice/triceAuxiliary.c
    trice/triceDoubleBuffer.c
#    trice/triceRingBuffer.c
#    trice/triceStackBuffer.c
#    trice/triceStaticBuffer.c
    trice/xtea.c

    User/triceLogDiagData.c
    User/triceCheck.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    SRC
    SRC/Core
#    SRC/Debug
    SRC/Peripheral/inc
    User
    ../../src
)

target_link_libraries(${PROJECT_NAME} toolchain)

target_link_options(${PROJECT_NAME} PRIVATE
    -T "${CMAKE_SOURCE_DIR}/SRC/Ld/Link.ld"
    -Wl,-Map=${PROJECT_NAME}.map
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary "$<TARGET_FILE:${PROJECT_NAME}>" ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} ARGS --only-keep-debug $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.debug
)
