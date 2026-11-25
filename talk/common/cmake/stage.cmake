# Common build for a talk stage. Include it from the stage's CMakeLists.txt after project().
#
# A stage directory is a self-contained snapshot:
#   main.cpp (or main.c), startup/, core/, soc/, bsp/<board>/   code shown on the slides
#   variants/<name>/<board>/                                    error demos: headers here shadow the normal ones,
#                                                               a .cpp here replaces the bsp/<board>/ file of the same name
#
# Set before including:
#   STAGE_VENDOR_STARTUP  ON: build with the vendor startup file instead of startup/startup.cpp (00, 01)
#   STAGE_VENDOR_SYSTEM   ON: compile the vendor SystemInit (early init hook)
#   STAGE_BOARDS          boards this stage supports (default: both)

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/boards.cmake)

set(STARTUP_VARIANT "" CACHE STRING "Variant directory under variants/ (error demos)")

if(NOT DEFINED STAGE_BOARDS)
    set(STAGE_BOARDS NucleoF411RE nRF54L15DK)
endif()
if(NOT STARTUP_BOARD IN_LIST STAGE_BOARDS)
    message(FATAL_ERROR "This stage supports only: ${STAGE_BOARDS}")
endif()

set(STAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VENDOR_DIR ${CMAKE_CURRENT_LIST_DIR}/../vendor)

if(STARTUP_BOARD STREQUAL "NucleoF411RE")
    set(STAGE_SOC_DIRS ${STAGE_DIR}/soc/arm ${STAGE_DIR}/soc/ST/STM32F411)
    set(VENDOR_STARTUP ${VENDOR_DIR}/st/startup_stm32f411xe.s)
    set(VENDOR_LDSCRIPT ${STARTUP_LDSCRIPT})
else()
    set(STAGE_SOC_DIRS ${STAGE_DIR}/soc/arm ${STAGE_DIR}/soc/Nordic/nRF54L)
    set(VENDOR_STARTUP ${VENDOR_DIR}/nordic/gcc_startup_nrf54l15_application.S)
    set(VENDOR_LDSCRIPT ${VENDOR_DIR}/nordic/nrf54l15_xxaa_application.ld)
endif()

set(VARIANT_DIR "")
if(STARTUP_VARIANT)
    set(VARIANT_DIR ${STAGE_DIR}/variants/${STARTUP_VARIANT}/${STARTUP_BOARD})
    if(NOT IS_DIRECTORY ${VARIANT_DIR})
        message(FATAL_ERROR "Variant '${STARTUP_VARIANT}' has no directory for ${STARTUP_BOARD}: ${VARIANT_DIR}")
    endif()
endif()

add_compile_definitions(${STARTUP_DEVICE_DEFINES})

if(STAGE_VENDOR_STARTUP)
    add_executable(firmware ${STAGE_DIR}/main.c ${VENDOR_STARTUP} ${STARTUP_VENDOR_SYSTEM} ${VENDOR_DIR}/syscalls.c)
    target_include_directories(firmware PRIVATE ${STARTUP_DRIVER_DIRS})
    startup_link(firmware ${VENDOR_LDSCRIPT})
    return()
endif()

# Board sources, with same-named files from the variant taking their place
file(GLOB BOARD_SOURCES ${STAGE_DIR}/bsp/${STARTUP_BOARD}/*.cpp)
if(VARIANT_DIR)
    file(GLOB VARIANT_SOURCES ${VARIANT_DIR}/*.cpp)
    foreach(src ${VARIANT_SOURCES})
        get_filename_component(name ${src} NAME)
        list(FILTER BOARD_SOURCES EXCLUDE REGEX "/${name}$")
    endforeach()
    list(APPEND BOARD_SOURCES ${VARIANT_SOURCES})
endif()

add_executable(firmware ${STAGE_DIR}/main.cpp ${STAGE_DIR}/startup/startup.cpp ${BOARD_SOURCES})
if(STAGE_VENDOR_SYSTEM)
    target_sources(firmware PRIVATE ${STARTUP_VENDOR_SYSTEM})
endif()

target_include_directories(firmware PRIVATE
        ${VARIANT_DIR}
        ${STAGE_DIR}/bsp/${STARTUP_BOARD}
        ${STAGE_SOC_DIRS}
        ${STAGE_DIR}/startup
        ${STAGE_DIR}/core
        ${STARTUP_DRIVER_DIRS}
)
startup_link(firmware ${STARTUP_LDSCRIPT})
