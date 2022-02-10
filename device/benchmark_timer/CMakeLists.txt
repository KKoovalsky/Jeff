add_library(benchmark_timer STATIC benchmark_timer.cpp)
target_include_directories(benchmark_timer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(benchmark_timer PUBLIC jungles::cpp_utils)
target_link_libraries(benchmark_timer PRIVATE cube os_utils)