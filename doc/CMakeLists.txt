#
# Build documentation
#
add_custom_command(OUTPUT doc.stamp
    COMMAND ${DOXYGEN} ./etc/Doxyfile
    COMMAND touch ./doc/doc.stamp
    MAIN_DEPENDENCY ../etc/Doxyfile
    DEPENDS ${SOURCE_FILES} ${HEADER_FILES}
    WORKING_DIRECTORY ..
)


#
# Add target "doc" if doxygen is availible
#
if(EXISTS ${DOXYGEN})
    add_custom_target(doc
        DEPENDS doc.stamp
    )
endif()


