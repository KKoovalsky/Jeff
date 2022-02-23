set(ARM_GNU_COMPILER_PATH_PREFIX ${ARM_GNU_TOOLCHAIN_PATH}/bin)

set(TRIPLET arm-none-eabi)

# Without that you will get linking error related to no _exit symbol when CMake tests gcc compile.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_NAME Generic)

if(DEFINED ENV{CC})
    set(CMAKE_C_COMPILER $ENV{CC})
else()
    set(CMAKE_C_COMPILER ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-gcc)
endif()

if(DEFINED ENV{CXX})
    set(CMAKE_CXX_COMPILER $ENV{CXX})
else()
    set(CMAKE_CXX_COMPILER ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-g++)
endif()

set(CMAKE_ASM_COMPILER ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-gcc)
set(CMAKE_LINKER ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-ld)

set(CMAKE_AR ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-ar)
set(CMAKE_OBJCOPY ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-objcopy)
set(CMAKE_OBJCOPY_BIN ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-objcopy)
set(CMAKE_RANLIB ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-ranlib)
set(CMAKE_STRIP ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-strip)
set(CMAKE_SIZE_BIN ${ARM_GNU_COMPILER_PATH_PREFIX}/${TRIPLET}-size)

string(CONCAT basic_flags
    " -Wall -Wextra"
    " -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16"
    " -fdata-sections -ffunction-sections"
    )

set(compile_c_flags "${basic_flags}")
set(compile_asm_flags "${basic_flags}")

set(extra_cxx_flags "-fexceptions")
set(compile_cxx_flags "${basic_flags} ${extra_cxx_flags}")

string(CONCAT extra_linker_flags
    " -specs=nosys.specs"
    " -Wl,--gc-sections"
)

set(linker_flags "${basic_flags} ${extra_linker_flags}")

set(CMAKE_C_FLAGS_INIT "${compile_c_flags}")
set(CMAKE_ASM_FLAGS_INIT  "${compile_asm_flags}")
set(CMAKE_CXX_FLAGS_INIT "${compile_cxx_flags}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${linker_flags}")
