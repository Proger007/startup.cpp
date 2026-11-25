cmake_minimum_required(VERSION 3.23)

project(01_vendor_typo C CXX ASM)

set(STAGE_VENDOR_STARTUP ON)
set(STARTUP_CRT0 ON)   # the vendor startup calls __libc_init_array / _start
set(STAGE_BOARDS NucleoF411RE)

include(../common/cmake/stage.cmake)
