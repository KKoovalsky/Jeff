##########################################################################################
# Functions and macros
##########################################################################################

##########################################################################################
# Main script
##########################################################################################
include(../../cmake/device_tests.cmake)

ProvideUnity()
ProvideJunglesCppUtils()
ProvideQDspLibrary()

add_subdirectory(setup)
add_subdirectory(test_serial_logger)
add_subdirectory(test_audio_sampler)
add_subdirectory(test_button)
add_subdirectory(test_audio_dac)
add_subdirectory(test_audio_chain)
add_subdirectory(test_filter_cutoff_setter_clock)
add_subdirectory(test_benchmark_timer)
add_subdirectory(test_distortion)
add_subdirectory(benchmarks)
