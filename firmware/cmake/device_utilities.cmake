##########################################################################################
# Functions and macro definition
##########################################################################################
function(AddCommandWhichHaltsDevice)

    add_custom_target(halt_device
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg -c "init; reset halt; exit"
        VERBATIM
    )

endfunction()

function(AddCommandWhichRunsDevice)

    add_custom_target(run_device
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg -c "init; reset run; exit"
        VERBATIM
    )

endfunction()

function(AddGdbServerRunner)

    add_custom_target(run_gdb_server
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg
        VERBATIM
    )

endfunction()


##########################################################################################
# MainScript
##########################################################################################
AddCommandWhichHaltsDevice()
AddCommandWhichRunsDevice()
AddGdbServerRunner()
