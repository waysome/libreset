
set(MP_FILES
    hierarchical-merge.mp
    right-rot.mp
    runtime-avl.mp
    single-merge.mp
)

set(mp_figs)

foreach(file_in ${MP_FILES})
    string(REPLACE ".mp" ".1" file_out ${file_in})
    add_custom_command(
        OUTPUT ${file_out}
        COMMAND mpost ${file_in}
        MAIN_DEPENDENCY ${file_in}
    )
    list(APPEND mp_figs ${file_out})
endforeach()


add_custom_target(figures DEPENDS ${mp_figs})


