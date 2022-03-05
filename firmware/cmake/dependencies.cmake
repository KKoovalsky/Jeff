function(ProvideCatch2)

    include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY    https://github.com/catchorg/Catch2.git
      GIT_TAG           5121660e7ff38e171ac01ded949502a5dba7570f)

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
        GIT_TAG 966243ab0bdbd216154a3fed4de8b65f07da4922
    )

    FetchContent_MakeAvailable(JunglesOsHelpers)

endfunction()

function(ProvideJunglesCppUtils)

    include(FetchContent)
    FetchContent_Declare(
        JunglesCppUtils
        GIT_REPOSITORY https://github.com/KKoovalsky/JunglesCppUtils.git
        GIT_TAG 8429958f77db10b3e4bab4eceebd7af04ff9a5fc)
    FetchContent_MakeAvailable(JunglesCppUtils)

endfunction()

function(ProvideQDspLibrary)

    option(Q_BUILD_EXAMPLES "Build Q library examples" OFF)
    option(Q_BUILD_TEST "Build Q library tests" OFF)
    option(Q_BUILD_IO "Build Q IO library" OFF)

    include(FetchContent)
    FetchContent_Declare(QDspLibrary
        GIT_REPOSITORY https://github.com/cycfi/q.git
        GIT_TAG bae1b93db5d85427b28054c604f838a61b783a26
    )

    FetchContent_MakeAvailable(QDspLibrary)
    target_compile_definitions(libq INTERFACE Q_DONT_USE_THREADS)

endfunction()

function(ProvideRingBufferLibrary)

    include(FetchContent)
    FetchContent_Declare(RingBuffer
        GIT_REPOSITORY https://github.com/jnk0le/Ring-Buffer
        GIT_TAG e93d9afd48c148964323acdef7ec73883f578bbc
    )

    FetchContent_MakeAvailable(RingBuffer)
    FetchContent_GetProperties(RingBuffer SOURCE_DIR RING_BUFFER_LIB_SRC_DIR)

    add_library(ring_buffer INTERFACE)
    target_include_directories(ring_buffer INTERFACE ${RING_BUFFER_LIB_SRC_DIR})

endfunction()

function(ProvideJunglesRingBufferLibrary)

    include(FetchContent)
    FetchContent_Declare(JunglesRingBuffer
        GIT_REPOSITORY https://github.com/KKoovalsky/RingBuffer
        GIT_TAG a5a277c8507c5b15eee68516ed1f2f3b8fa5a441
    )

    FetchContent_MakeAvailable(JunglesRingBuffer)

endfunction()
