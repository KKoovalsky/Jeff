add_executable(test_audio_dac_basic test_audio_dac_basic.cpp)
ToDeviceTest(test_audio_dac_basic test_audio_dac_basic)
target_link_libraries(test_audio_dac_basic PRIVATE
    audio_dac_with_dma os_utils serial_logger sampling_trigger_timer_impl benchmark_timer)

add_executable(test_audio_dac_generates_sine_1khz test_audio_dac_generates_sine_1khz.cpp)
ToDeviceTest(test_audio_dac_generates_sine_1khz test_audio_dac_generates_sine_1khz)
target_link_libraries(test_audio_dac_generates_sine_1khz PRIVATE
    audio_dac_with_dma jungles::os_helpers os_utils serial_logger sampling_trigger_timer_impl libq)