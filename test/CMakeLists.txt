SET(TEST_SOURCES
	test.cc
    lexical/Lexical.cc
    syntax/Syntax.cc
    syntax/AST.cc
    parser/Parser.cc
)

ADD_EXECUTABLE(utlc_test ${TEST_SOURCES})
INCLUDE_DIRECTORIES(../inc)
ADD_CUSTOM_TARGET(run_test COMMAND ${CMAKE_BINARY_DIR}/test/utlc_test DEPENDS utlc_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
