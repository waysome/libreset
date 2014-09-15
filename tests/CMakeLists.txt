
find_package(Check)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/libreset
)


#
# Test program
#
add_executable(check_tests EXCLUDE_FROM_ALL
    tests.c
)

target_link_libraries(check_tests
    ${CHECK_LIBRARY}
    ${PROJECT_SOURCE_DIR}/src/libreset.so)


#
# Ultimately, the test
#
if(${CHECK_FOUND})
    add_test("build_check_test" "${CMAKE_COMMAND}"
        --build ${CMAKE_BINARY_DIR}
        --target check_tests
    )
    add_test("check_tests" check_tests)
    set_tests_properties(check_tests PROPERTIES DEPENDS build_check_tests)
else()
    message("Check framework not found")
endif()

