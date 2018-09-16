# Test random
add_executable(random_test random_test.cpp)
add_test(random_test random_test)
target_link_libraries(random_test gtest_main sim_util)
