include(cube_sources.cmake)
set(cmsis_os_dir Jeff/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2)

add_library(cube STATIC ${cube_sources} ${cmsis_os_dir}/cmsis_os2.c)

target_compile_definitions(cube PUBLIC USE_HAL_DRIVER USE_FULL_LL_DRIVER STM32L432xx)
target_include_directories(cube PUBLIC
    Jeff/Core/Inc
    Jeff/Drivers/STM32L4xx_HAL_Driver/Inc
    Jeff/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    Jeff/Drivers/CMSIS/Include
    ${cmsis_os_dir}
)
target_link_libraries(cube PRIVATE freertos)
target_compile_options(cube PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>)

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE Jeff/Core/Inc)
