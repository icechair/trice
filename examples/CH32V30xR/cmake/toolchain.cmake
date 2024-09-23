set(CMAKE_SYSTEM_NAME Generic-ELF)
set(CMAKE_SYSTEM_PROCESSOR rv32imafc_zicsr)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_CROSSCOMPILING 1)

set(target_triplet "riscv-none-elf")
set(toolchain_provider "xpacks/@xpack-dev-tools")

set(xpack_path ${CMAKE_SOURCE_DIR}/${toolchain_provider}/${target_triplet}-gcc/.content/bin)
set(xpack_ext "")
message(INFO " CMAKE_HOST_SYSTEM_NAME = ${CMAKE_HOST_SYSTEM_NAME}")
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
set(xpack_ext ".exe")
endif()
set(CMAKE_C_COMPILER ${xpack_path}/${target_triplet}-gcc${xpack_ext})
set(CMAKE_CXX_COMPILER ${xpack_path}/${target_triplet}-g++${xpack_ext})
set(CMAKE_AR ${xpack_path}/${target_triplet}-ar${xpack_ext})
set(CMAKE_ASM_COMPILER ${xpack_path}/${target_triplet}-gcc${xpack_ext})
set(CMAKE_OBJCOPY ${xpack_path}/${target_triplet}-objcopy${xpack_ext})
set(CMAKE_OBJDUMP ${xpack_path}/${target_triplet}-objdump${xpack_ext})


add_library(toolchain INTERFACE)

target_compile_features(toolchain INTERFACE
    c_std_99
    cxx_std_20
)

target_link_options(toolchain INTERFACE
    -march=rv32imac_zicsr
    -mabi=ilp32
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,--print-memory-usage
    -Wl,--gc-sections
    -nostartfiles
    -Xlinker
)

target_compile_options(toolchain INTERFACE
    -march=rv32imac_zicsr
    -mabi=ilp32
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    -Og
    -g3
    -Wall
    -fno-common
    -finline-small-functions
    -findirect-inlining
    # no exceptions
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-non-call-exceptions>
    #no rtti
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    #no destructors after main
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
    #freestanding environment, no stdlib present
    -ffreestanding
    -Wno-comment
    $<$<COMPILE_LANGUAGE:C>:-Werror-implicit-function-declaration>
    #-flto=2
    #-fno-fat-lto-objects
)

set(TOOLCHAIN_LIB ON)
