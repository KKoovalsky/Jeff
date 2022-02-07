add_executable(distortion_app distortion_app.cpp)
target_link_libraries(distortion_app PRIVATE
    jeff_common
    jeff_app_wrapper)