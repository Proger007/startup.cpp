cmake_minimum_required(VERSION 3.23)

project(00_vendor_baseline C CXX ASM)

set(STAGE_VENDOR_STARTUP ON)
set(STARTUP_CRT0 ON)   # the vendor startup calls __libc_init_array / _start

include(../common/cmake/stage.cmake)
