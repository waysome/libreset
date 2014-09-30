#
# Building libreset
#


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/libreset)


#
# We use the var SOURCE_FILES to hold all source files to be linked together
# into libreset. Additionally, checks checky may be performed on that source
# files.
#
set(SOURCE_FILES
    libreset/avl/avl_cardinality.c
    libreset/avl/avl_select.c
    libreset/avl/avl_is_subset.c
    libreset/avl/base.c
    libreset/avl/common.c
    libreset/avl/node_cache.c
    libreset/bloom.c
    libreset/ht/base.c
    libreset/ht/ht_cardinality.c
    libreset/ht/ht_equal.c
    libreset/ht/ht_select.c
    libreset/ll/base.c
    libreset/ll/ll_count.c
    libreset/ll/ll_equal.c
    libreset/ll/ll_select.c
    libreset/ll/ll_is_subset.c
    libreset/set.c
)

#
# General compile options
#
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunused -Wformat")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

if(${HARD_MODE})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wno-error=unused-function")
endif()

#
# Debugging options
#
if(${DEBUG_OUTPUT})
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
endif()

#
# Libreset will be a shared object
#
add_library(reset SHARED ${SOURCE_FILES})

install(TARGETS reset LIBRARY DESTINATION lib)


