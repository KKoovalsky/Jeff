# Unity library creates "unity" target which we will edit here to include custom configuration, which
# is included inside "unity_config.h" file.
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_CONFIG_H)
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(unity_putchar STATIC unity_putchar.cpp)
target_link_libraries(unity_putchar PRIVATE serial_logger)

# We could link 'unity_putchar' to 'unity' library, as PRIVATE dependency (which it is actually), but
# 'unity' is installed (using 'install' command), and CMake expects all the dependencies to be installable as well.
# Otherwise, an error will be outputted:
#
#   CMake Error: install(EXPORT "unityTargets" ...) includes target "unity" which requires target
#   "unity_putchar" that is not in any export set.
#
# This is why we create an interface library which bundles both libraries, simulating the typical linkage approach.
add_library(unity_interface INTERFACE)
target_link_libraries(unity_interface INTERFACE unity unity_putchar)

add_library(unity_putchar_no_output STATIC unity_putchar_empty.cpp)
# This adds a target similar to 'unity_interface' without output printing of the tests result.
add_library(unity_interface_no_output INTERFACE)
target_link_libraries(unity_interface_no_output INTERFACE unity unity_putchar_no_output)

add_library(device_test_setup STATIC test_runner.cpp)
target_link_libraries(device_test_setup PRIVATE jungles::os_helpers serial_logger)