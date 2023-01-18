
set(TEST_NAME test_soww_widgets)
add_executable(${TEST_NAME} ../TestSuiteMain.cpp TestSoWwGLArea.cpp)
target_link_libraries(${TEST_NAME}  SoWw )

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})