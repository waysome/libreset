#
# Building libreset
#


#
# We use the var SOURCE_FILES to hold all source files to be linked together
# into libreset. Additionally, checks checky may be performed on that source
# files.
#
set(SOURCE_FILES
    libreset/ll.c
)

#
# Libreset will be a shared object
#
add_library(reset SHARED ${SOURCE_FILES})


