cmake_minimum_required(VERSION 3.23)

project(blink C CXX ASM)

include(cmake/boards.cmake)

if(STARTUP_BOARD STREQUAL "NucleoF411RE")
    set(DRIVER_SOURCES Drivers/ST/stm32f4_handler_dummy.cpp)
    set(DRIVER_DIRS Drivers/ST)
else()
    set(DRIVER_SOURCES Drivers/Nordic/nrf54_handler_dummy.cpp)
    set(DRIVER_DIRS Drivers/Nordic)
endif()

include_directories(
        bsp/${STARTUP_BOARD}
        ${STARTUP_SOC_DIRS}
        startup
        core
        ${DRIVER_DIRS}
        ${STARTUP_DRIVER_DIRS}
)
add_compile_definitions(${STARTUP_DEVICE_DEFINES})

add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        startup/startup.cpp
        ${DRIVER_SOURCES}
        ${STARTUP_VENDOR_SYSTEM}
)
startup_link(${CMAKE_PROJECT_NAME} ${STARTUP_LDSCRIPT})
