add_library(audio_sampler_with_dma STATIC audio_sampler_with_dma.cpp)
target_include_directories(audio_sampler_with_dma PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(audio_sampler_with_dma PUBLIC jeff_common jungles::os_helpers freertos)
target_link_libraries(audio_sampler_with_dma PRIVATE cube os_utils)
