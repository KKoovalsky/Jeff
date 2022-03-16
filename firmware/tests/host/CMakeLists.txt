# =============================================================================
# Macro definitions
# =============================================================================
function(CreateHostTests)

    add_executable(jeff_host_tests
        test_audio_chain.cpp
        test_distortion.cpp
    )
    target_link_libraries(jeff_host_tests PRIVATE jeff_common Catch2::Catch2WithMain)

    add_test(NAME jeff_host COMMAND valgrind --leak-check=full $<TARGET_FILE:jeff_host_tests>)

endfunction()

# =============================================================================
# Main script
# =============================================================================

ProvideCatch2()
ProvideJunglesOsHelpers()
CreateHostTests()
