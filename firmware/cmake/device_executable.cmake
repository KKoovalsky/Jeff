##########################################################################################
# Function definitions
##########################################################################################

function(MakeDeviceExecutable target_name)

    LinkCustomTerminate(${target_name})
    LinkHardFaultHandler(${target_name})
    LinkDeviceSpecificDetails(${target_name})
    GenerateHexAfterBuild(${target_name})
    PrintBinarySizeAfterBuild(${target_name})
    AddFlashTargets(${target_name})

endfunction()

function(LinkCustomTerminate)

    set(device_dir "${PROJECT_ROOT_DIR}/device")
    set(custom_terminate_file "${device_dir}/custom_terminate.cpp")
    target_sources(${target_name} PRIVATE ${custom_terminate_file})

endfunction()

function(LinkHardFaultHandler)

    set(device_dir "${PROJECT_ROOT_DIR}/device")
    set(hard_fault_handler_file "${device_dir}/hard_fault_handler.cpp")
    target_sources(${target_name} PRIVATE ${hard_fault_handler_file})

endfunction()

function(LinkDeviceSpecificDetails target_name)
set(cube_generated_files_dir "${PROJECT_ROOT_DIR}/device/cube/Jeff")

    set(linker_script "${cube_generated_files_dir}/STM32L432KCUx_FLASH.ld")
    target_link_options(${target_name} PRIVATE -T${linker_script})

    target_link_libraries(${target_name} PRIVATE device_specific cube)

    target_sources(${target_name} PRIVATE $<TARGET_OBJECTS:custom_alloc>)

endfunction()

function(GenerateHexAfterBuild target_name)

    set(target_hex_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.hex)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target_name}> ${target_hex_file}
        BYPRODUCTS ${target_hex_file}
        COMMENT "Generating executable HEX file for ${target_name}"
    )

endfunction()

function(PrintBinarySizeAfterBuild target_name)

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_SIZE_BIN} $<TARGET_FILE:${target_name}>
        COMMENT "Section sizes of the executable binary"
    )

endfunction()

function(AddFlashTargets target_name)

    find_program(OPENOCD_PROGRAM NAMES openocd REQUIRED)

    add_custom_target(${target_name}-flash
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg
            -c "program $<TARGET_FILE:${target_name}> ; reset ; exit"
        VERBATIM
    )

    add_custom_target(${target_name}-flash_no_run
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg
            -c "program $<TARGET_FILE:${target_name}> ; reset halt ; exit"
        VERBATIM
    )

endfunction()

function(CreateDeviceSpecificLibraries)

    set(cube_generated_files_dir "${PROJECT_ROOT_DIR}/device/cube/Jeff")
    set(startup_file "${cube_generated_files_dir}/startup_stm32l432xx.s")

    add_library(device_specific STATIC ${startup_file})

    set(device_dir "${PROJECT_ROOT_DIR}/device")
    set(custom_alloc_file "${device_dir}/custom_alloc.cpp")
    add_library(custom_alloc OBJECT ${custom_alloc_file})

endfunction()

##########################################################################################
# Main script
##########################################################################################
CreateDeviceSpecificLibraries()