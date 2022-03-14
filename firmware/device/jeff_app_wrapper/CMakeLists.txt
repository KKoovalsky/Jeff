add_library(jeff_app_wrapper STATIC jeff_app_wrapper.cpp)
target_include_directories(jeff_app_wrapper PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(jeff_app_wrapper PUBLIC jeff_common)
target_link_libraries(jeff_app_wrapper PRIVATE
    sampling_trigger_timer_impl audio_dac_with_dma audio_sampler_with_dma filter_cutoff_setter_clock
)

set(JEFF_TRACE_AND_DUMP_EVENTS OFF CACHE BOOL "Enables or disables tracing and dumping events over serial")

if(JEFF_TRACE_AND_DUMP_EVENTS)
    target_compile_definitions(jeff_app_wrapper PRIVATE JEFF_TRACE_AND_DUMP_EVENTS)
    target_link_libraries(jeff_app_wrapper PRIVATE benchmark_timer event_periodic_dumper)
endif()
