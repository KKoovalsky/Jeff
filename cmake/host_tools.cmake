function(EnsureRubyIsInstalled ruby_program_out_var)

    find_program(ruby_program ruby REQUIRED)
    set(${ruby_program_out_var} ${ruby_program} PARENT_SCOPE)

endfunction()

function(EnsurePython3IsInstalled python3_program_out_var)

    find_program(python3_program python3 REQUIRED)
    set(${python3_program_out_var} ${python3_program} PARENT_SCOPE)

endfunction()