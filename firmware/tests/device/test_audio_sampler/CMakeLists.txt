##########################################################################################
# Functions definition
##########################################################################################
function(CreateSineWaveSampleTest name)

    set(test_source_file ${name}.cpp)
    add_executable(${name} ${test_source_file})
    target_link_libraries(${name} PRIVATE test_sample_sine)
    ToDeviceTest(${name} ${name})

endfunction()
##########################################################################################
# Main script
##########################################################################################

add_executable(test_audio_sampler_basic test_audio_sampler_basic.cpp)
ToDeviceTest(test_audio_sampler_basic test_audio_sampler_basic)
target_link_libraries(test_audio_sampler_basic PRIVATE
    audio_sampler_with_dma serial_logger sampling_trigger_timer_impl benchmark_timer os_utils)

add_library(test_sample_sine test_sample_sine.cpp)
target_link_libraries(test_sample_sine PRIVATE
    unity audio_sampler_with_dma serial_logger cmsis_dsp_float sampling_trigger_timer_impl)

CreateSineWaveSampleTest(test_sample_sine_100hz)
CreateSineWaveSampleTest(test_sample_sine_1khz)
CreateSineWaveSampleTest(test_sample_sine_10khz)
CreateSineWaveSampleTest(test_sample_sine_20khz)