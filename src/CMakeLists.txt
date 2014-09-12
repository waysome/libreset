#
# Building libreset
#


include_directories(${PROJECT_SOURCE_DIR}/include)


#
# We use the var SOURCE_FILES to hold all source files to be linked together
# into libreset. Additionally, checks checky may be performed on that source
# files.
#
set(SOURCE_FILES
    libreset/avl.c
    libreset/ll.c
)

#
# HEADER_FILES holds all headers to process.
#
set(HEADER_FILES
    libreset/avl.h
    libreset/ht.h
    libreset/ll.h
    libreset/util/attributes.h
    libreset/util/likely.h
    libreset/util/macros.h
)


#
# General compile options
#
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunused -Wformat")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

#
# Libreset will be a shared object
#
add_library(reset SHARED ${SOURCE_FILES})


#
# Style checks
#
foreach(file_in ${SOURCE_FILES} ${HEADER_FILES})
    add_test(
        NAME style_${file_in}
        COMMAND ${CHECKSTYLE} ${file_in}
    )
endforeach()


