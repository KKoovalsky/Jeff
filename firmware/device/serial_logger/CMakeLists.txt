##########################################################################################
# Functions
##########################################################################################

function(ProvidePrintfLibrary)

    include(FetchContent)

    set(BUILD_STATIC_LIBRARY ON)
    set(SUPPORT_LONG_LONG OFF)
    set(ALIAS_STANDARD_FUNCTION_NAMES OFF)
    set(DEFAULT_FLOAT_PRECISION "3")

    FetchContent_Declare(printf_library
        GIT_REPOSITORY https://github.com/eyalroz/printf.git
        GIT_TAG v5.0.0
    )
    FetchContent_MakeAvailable(printf_library)

    FetchContent_GetProperties(printf_library SOURCE_DIR printf_source_dir)
    target_include_directories(printf PUBLIC $<BUILD_INTERFACE:${printf_source_dir}>)

endfunction()

##########################################################################################
# Main script
##########################################################################################

add_library(serial_logger STATIC serial_logger.cpp)
target_include_directories(serial_logger PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(serial_logger PUBLIC jeff_common)
target_link_libraries(serial_logger PRIVATE cube)

ProvidePrintfLibrary()
target_link_libraries(serial_logger PRIVATE printf)