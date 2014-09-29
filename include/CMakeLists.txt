#
# We use a variable for the interface headers
#
set(IF_HEADERS
    libreset/attributes.h
    libreset/hash.h
    libreset/set.h
)

#
# Install libreset headers
#
install(FILES ${IF_HEADERS} DESTINATION include/libreset)

