file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(pa_tst ${TEST_SOURCES})

add_test(NAME pa_tst COMMAND ${ECE_EXECUTABLE_NAME}_tst)

target_link_libraries(pa_tst PUBLIC ${ECE_EXECUTABLE_NAME}_lib gtest)
