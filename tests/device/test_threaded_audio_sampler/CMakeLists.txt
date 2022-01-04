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

add_executable(test_threaded_audio_sampler test_threaded_audio_sampler.cpp)
ToDeviceTest(test_threaded_audio_sampler test_threaded_audio_sampler)
target_link_libraries(test_threaded_audio_sampler PRIVATE
    threaded_audio_sampler jungles::os_helpers os_utils serial_logger)

add_library(test_sample_sine test_sample_sine.cpp)
target_link_libraries(test_sample_sine PRIVATE
    unity threaded_audio_sampler serial_logger libq jungles::os_helpers jungles::cpp_utils)

CreateSineWaveSampleTest(test_sample_sine_100hz)
CreateSineWaveSampleTest(test_sample_sine_1khz)
CreateSineWaveSampleTest(test_sample_sine_10khz)
CreateSineWaveSampleTest(test_sample_sine_20khz)