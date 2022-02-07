add_executable(test_audio_chain test_audio_chain.cpp)
ToDeviceTest(test_audio_chain test_audio_chain)
target_link_libraries(test_audio_chain PRIVATE
    jeff_common
    jeff_app_wrapper
    serial_logger
)