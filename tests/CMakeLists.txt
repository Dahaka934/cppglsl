add_executable(Tests test.cpp)
target_link_libraries(Tests PUBLIC glsl)

add_test(NAME Tests COMMAND Tests)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
