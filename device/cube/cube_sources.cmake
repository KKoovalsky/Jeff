set(cube_core_src_dir Jeff/Core/Src/)
set(cube_core_sources
    main.c
    gpio.c
    freertos.c
    stm32l4xx_it.c
    stm32l4xx_hal_msp.c
    stm32l4xx_hal_timebase_tim.c
    system_stm32l4xx.c
    usart.c
    adc.c
    dma.c
)
list(TRANSFORM cube_core_sources PREPEND ${cube_core_src_dir})

set(cube_drivers_src_dir Jeff/Drivers/STM32L4xx_HAL_Driver/Src/)
set(cube_drivers_sources
    stm32l4xx_hal.c
    stm32l4xx_hal_rcc.c
    stm32l4xx_hal_rcc_ex.c
    stm32l4xx_hal_flash.c
    stm32l4xx_hal_flash_ex.c
    stm32l4xx_hal_flash_ramfunc.c
    stm32l4xx_hal_gpio.c
    stm32l4xx_hal_dma.c
    stm32l4xx_hal_dma_ex.c
    stm32l4xx_hal_pwr.c
    stm32l4xx_hal_pwr_ex.c
    stm32l4xx_hal_cortex.c
    stm32l4xx_hal_exti.c
    stm32l4xx_hal_tim.c
    stm32l4xx_hal_tim_ex.c
    stm32l4xx_hal_uart.c
    stm32l4xx_ll_gpio.c
    stm32l4xx_ll_adc.c
)
list(TRANSFORM cube_drivers_sources PREPEND ${cube_drivers_src_dir})

set(cube_sources ${cube_core_sources} ${cube_drivers_sources})
