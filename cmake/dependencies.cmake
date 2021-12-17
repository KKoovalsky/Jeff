function(ProvideCatch2)

    include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        4436a604566f18091489b55af215b8d32afe657a)

    # TODO: PREFIX, subbuild and src (expect build) can be the same for host and the device build. We need to check out
    # what is subbuild and whether it is system dependent. See how is PREFIX, SUBBUILD_DIR, ... used in ProvideLlvm()

    FetchContent_MakeAvailable(Catch2)

endfunction()

function(ProvideArmGnuToolchain)

    include(FetchContent)

    set(COMMON_DEPENDENCIES_PATH ${CMAKE_SOURCE_DIR}/../build/common_dependencies)

    FetchContent_Declare(
        ArmGnuToolchain
        URL https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
        URL_HASH MD5=2383e4eb4ea23f248d33adc70dc3227e
        PREFIX ${COMMON_DEPENDENCIES_PATH}
        SUBBUILD_DIR ${COMMON_DEPENDENCIES_PATH}/armgnutoolchain-subbuild
        SOURCE_DIR ${COMMON_DEPENDENCIES_PATH}/armgnutoolchain-src
        BINARY_DIR ${COMMON_DEPENDENCIES_PATH}/armgnutoolchain-build
    )

    FetchContent_MakeAvailable(ArmGnuToolchain)

    FetchContent_GetProperties(ArmGnuToolchain SOURCE_DIR ARM_GNU_TOOLCHAIN_SOURCE_DIR)

    set(ARM_GNU_TOOLCHAIN_PATH "${ARM_GNU_TOOLCHAIN_SOURCE_DIR}" CACHE PATH "Path to the ARM GNU toolchain")
    set(ARM_GNU_TOOLCHAIN_GCC_VERSION "10.3.1" CACHE STRING "Version of the GCC bundled with the ARM GNU toolchain")

endfunction()

function(DownloadAndPopulateJunglesCMakeHelpers)

    include(FetchContent)

    FetchContent_Declare(
        cmake_helpers
        GIT_REPOSITORY https://github.com/KKoovalsky/CMakeHelpers.git
        GIT_TAG e81be067115c349a55715e325ebed98795d55cfa
    )
    FetchContent_MakeAvailable(cmake_helpers)

endfunction()


function(ProvideFreertos)

    JunglesHelpers_DownloadAndPopulateFreeRTOSKernel(freertos V10.4.3 heap_4)
    target_include_directories(freertos PUBLIC ${FREERTOS_SOURCE_DIR}/portable/GCC/ARM_CM4F)
    target_sources(freertos PUBLIC ${FREERTOS_SOURCE_DIR}/portable/GCC/ARM_CM4F/port.c)

    # freertos_config shall be supplied externally. It shall contain at least include path to the FreeRTOSConfig.h.
    target_link_libraries(freertos PUBLIC freertos_config)

endfunction()

function(ProvideUnity)

    include(FetchContent)
    FetchContent_Declare(unity_project
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.5.2
    )
    FetchContent_MakeAvailable(unity_project)

    FetchContent_GetProperties(unity_project SOURCE_DIR unity_source_dir)

    set(UNITY_SOURCE_DIR ${unity_source_dir} PARENT_SCOPE)

endfunction()

function(ProvideJunglesOsHelpers)

    include(FetchContent)
    FetchContent_Declare(JunglesOsHelpers
        GIT_REPOSITORY https://github.com/KKoovalsky/JunglesOsHelpers.git
        GIT_TAG e506c2c7a71e1007f8c52a5e2683f8a4e87e7735
    )

    FetchContent_MakeAvailable(JunglesOsHelpers)
    target_link_libraries(JunglesOsHelpers INTERFACE freertos)

endfunction()