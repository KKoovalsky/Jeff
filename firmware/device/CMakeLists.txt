cmake_minimum_required(VERSION 3.21)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build configuration")

file(REAL_PATH ${CMAKE_CURRENT_LIST_DIR}/../ PROJECT_ROOT_DIR)

include(../cmake/dependencies.cmake)
ProvideArmGnuToolchain()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../cmake/arm_gnu_gcc_device_toolchain_file.cmake CACHE PATH
    "Path to the CMake's toolchain file")

project(JeffDevice LANGUAGES CXX C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)

DownloadAndPopulateJunglesCMakeHelpers()
ProvideFreertos()
ProvideJunglesOsHelpers()
target_link_libraries(JunglesOsHelpers INTERFACE freertos)

include(../cmake/device_executable.cmake)
include(../cmake/host_tools.cmake)
include(../cmake/device_utilities.cmake)

add_subdirectory(../src/ src)
add_subdirectory(cube)

add_subdirectory(os_utils)
add_subdirectory(serial_logger)
add_subdirectory(sampling_trigger_timer_impl)
add_subdirectory(audio_sampler_with_dma)
add_subdirectory(button)
add_subdirectory(audio_dac_with_dma)
add_subdirectory(jeff_app_wrapper)
add_subdirectory(distortion_app)
add_subdirectory(filter_cutoff_setter_clock)
add_subdirectory(benchmark_timer)
add_subdirectory(event_periodic_dumper)

add_executable(firmware main.cpp)
MakeDeviceExecutable(firmware)

enable_testing()
add_subdirectory(../tests/device tests/device)
