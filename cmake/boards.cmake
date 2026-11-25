# Board selection and build options shared by the main project and by talk/ stages.
#
#   STARTUP_BOARD     NucleoF411RE | nRF54L15DK
#   STARTUP_LIBC      nano | full | none   (newlib-nano, full newlib, -nostdlib)
#   STARTUP_BUILTINS  OFF adds -fno-builtin, so copy/zero loops stay loops
#   STARTUP_LTO       link-time optimization
#   STARTUP_CRT0      link the C library start files (crt0, crti). OFF for startup.cpp: it replaces them

set(STARTUP_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

set(STARTUP_BOARD "NucleoF411RE" CACHE STRING "Target board")
set_property(CACHE STARTUP_BOARD PROPERTY STRINGS NucleoF411RE nRF54L15DK)
set(STARTUP_LIBC "nano" CACHE STRING "C library")
set_property(CACHE STARTUP_LIBC PROPERTY STRINGS nano full none)
option(STARTUP_BUILTINS "Let GCC replace copy/zero loops with memcpy/memset" OFF)
option(STARTUP_LTO "Link-time optimization" OFF)
option(STARTUP_CRT0 "Link the C library start files" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(STARTUP_BOARD STREQUAL "NucleoF411RE")
    set(STARTUP_CPU_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g)
    set(STARTUP_DEVICE_DEFINES STM32F411xE)
    set(STARTUP_SOC_DIRS ${STARTUP_ROOT}/soc/arm ${STARTUP_ROOT}/soc/ST/STM32F411)
    set(STARTUP_DRIVER_DIRS ${STARTUP_ROOT}/Drivers/CMSIS/Include ${STARTUP_ROOT}/Drivers/ST/Include)
    set(STARTUP_VENDOR_SYSTEM ${STARTUP_ROOT}/Drivers/ST/Source/system_stm32f4xx.c)
    set(STARTUP_LDSCRIPT ${STARTUP_ROOT}/bsp/NucleoF411RE/STM32F411RETx_FLASH.ld)
elseif(STARTUP_BOARD STREQUAL "nRF54L15DK")
    set(STARTUP_CPU_FLAGS -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -g)
    set(STARTUP_DEVICE_DEFINES NRF54L15_XXAA NRF_APPLICATION)
    set(STARTUP_SOC_DIRS ${STARTUP_ROOT}/soc/arm ${STARTUP_ROOT}/soc/Nordic/nRF54L)
    set(STARTUP_DRIVER_DIRS ${STARTUP_ROOT}/Drivers/CMSIS/Include ${STARTUP_ROOT}/Drivers/Nordic)
    set(STARTUP_VENDOR_SYSTEM ${STARTUP_ROOT}/Drivers/Nordic/system_nrf54l.c)
    set(STARTUP_LDSCRIPT ${STARTUP_ROOT}/bsp/nRF54L15DK/nrf54_xxaa.ld)
else()
    message(FATAL_ERROR "Unknown STARTUP_BOARD '${STARTUP_BOARD}'")
endif()

add_compile_options(${STARTUP_CPU_FLAGS} -Os -Wall -ffunction-sections -fdata-sections)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-rtti> $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
                    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)
if(STARTUP_BUILTINS)
    # GCC 13 rewrites the copy/zero loops into memcpy/memset by default at -Os; GCC 15.3 kept the loops
    # in our build, so ask for the rewrite explicitly
    add_compile_options(-ftree-loop-distribute-patterns)
else()
    add_compile_options(-fno-builtin)
endif()

add_link_options(${STARTUP_CPU_FLAGS} -Wl,--gc-sections -Wl,--print-memory-usage)
if(STARTUP_LIBC STREQUAL "nano")
    add_link_options(--specs=nano.specs --specs=nosys.specs)
elseif(STARTUP_LIBC STREQUAL "full")
    add_link_options(--specs=nosys.specs)
elseif(STARTUP_LIBC STREQUAL "none")
    add_link_options(-nostdlib)
    link_libraries(gcc)
else()
    message(FATAL_ERROR "Unknown STARTUP_LIBC '${STARTUP_LIBC}'")
endif()

if(NOT STARTUP_CRT0)
    add_link_options(-nostartfiles)
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${STARTUP_LTO})

# Linker script, its directory (for INCLUDE), and a map file next to the ELF.
function(startup_link target ldscript)
    get_filename_component(ld_dir ${ldscript} DIRECTORY)
    target_link_options(${target} PRIVATE -T${ldscript} -L${ld_dir} -Wl,-Map=$<TARGET_FILE_DIR:${target}>/${target}.map)
    set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS ${ldscript})
endfunction()
