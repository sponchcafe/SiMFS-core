# Test io
add_executable(io_test io_test.cpp)
add_test(io_test io_test)
target_link_libraries(io_test gtest_main)
target_link_libraries(io_test sim_cmdutils)

# Test options
add_executable(options_test options_test.cpp)
add_test(options_test options_test)
target_link_libraries(options_test gtest_main)
target_link_libraries(options_test sim_cmdutils)
