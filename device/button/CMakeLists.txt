add_library(button STATIC button.cpp)
target_include_directories(button PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(button PRIVATE cube os_utils)