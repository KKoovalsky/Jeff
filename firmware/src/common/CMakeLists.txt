add_library(jeff_common INTERFACE)
target_include_directories(jeff_common INTERFACE ${CMAKE_CURRENT_LIST_DIR})

ProvideJunglesRingBufferLibrary()
target_link_libraries(jeff_common INTERFACE jungles::ring_buffer)
