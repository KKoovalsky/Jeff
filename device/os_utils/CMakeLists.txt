add_library(os_utils STATIC os_waiters.cpp)
target_include_directories(os_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(os_utils PRIVATE freertos)