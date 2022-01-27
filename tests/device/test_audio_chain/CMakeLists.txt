add_executable(test_audio_chain test_audio_chain.cpp)
ToDeviceTest(test_audio_chain test_audio_chain)
target_link_libraries(test_audio_chain PRIVATE
    jeff_common
    sampling_trigger_timer_impl audio_dac_impl threaded_audio_sampler
    jungles::os_helpers os_utils serial_logger
)
