add_executable(test_circleq test_circleq.c)
target_link_libraries(test_circleq ${CUNIT_LIBRARIES})
target_include_directories(test_circleq BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CUNIT_INCLUDE_DIRS}
)
target_compile_options(test_circleq PRIVATE ${CIRCLEQ_C_FLAGS})
add_test(NAME test_circleq COMMAND test_circleq)
