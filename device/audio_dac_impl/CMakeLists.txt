add_library(audio_dac_impl STATIC audio_dac_impl.cpp)
target_include_directories(audio_dac_impl PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(audio_dac_impl PUBLIC jeff_common jungles::cpp_utils)