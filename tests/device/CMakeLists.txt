##########################################################################################
# Functions and macros
##########################################################################################

##########################################################################################
# Main script
##########################################################################################
include(../../cmake/device_tests.cmake)

ProvideUnity()
ProvideQDspLibrary()
ProvideJunglesCppUtils()

add_subdirectory(setup)
add_subdirectory(test_serial_logger)
add_subdirectory(test_threaded_audio_sampler)
add_subdirectory(test_button)
