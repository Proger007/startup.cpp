set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                arm-none-eabi-)

find_program(ARM_GCC ${TOOLCHAIN_PREFIX}gcc)
if(NOT ARM_GCC)
  message(FATAL_ERROR "${TOOLCHAIN_PREFIX}gcc not in PATH")
endif()

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

# No CPU flags here: they depend on the board and are set in cmake/boards.cmake
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
