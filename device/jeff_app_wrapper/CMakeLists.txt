add_library(jeff_app_wrapper STATIC jeff_app_wrapper.cpp)
target_include_directories(jeff_app_wrapper PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(jeff_app_wrapper PUBLIC jeff_common)
target_link_libraries(jeff_app_wrapper PRIVATE
    sampling_trigger_timer_impl threaded_audio_dac threaded_audio_sampler filter_cutoff_setter_clock
    os_utils
    jungles::os_helpers)
