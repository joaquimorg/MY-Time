cmake_minimum_required(VERSION 3.0.0)
project(mytime VERSION 0.1.0 LANGUAGES C CXX ASM)

#set(CMAKE_C_STANDARD 99)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NRF_TARGET "nrf52")

if (NOT ARM_NONE_EABI_TOOLCHAIN_PATH)
  message(FATAL_ERROR "The path to the toolchain (arm-none-eabi) must be specified on the command line (add -DARM_NONE_EABI_TOOLCHAIN_PATH=<path>")
endif ()


add_subdirectory(src)
