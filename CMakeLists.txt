cmake_minimum_required(VERSION 3.21)
project(circleq NONE)

find_program(ASTYLE astyle)
if(ASTYLE)
    add_custom_target(astyle
        VERBATIM
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${ASTYLE} --options=${CMAKE_SOURCE_DIR}/.astylerc -Q -n circleq.h
    )
endif()

install(FILES circleq.h DESTINATION include)
