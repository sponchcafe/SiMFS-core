add_library(random src/random.cpp)
target_include_directories(random PUBLIC ./include)

# Test excecutable
add_executable(test_random tests/test_random.cpp)
target_link_libraries(test_random random)
target_link_libraries(test_random gtest_main)
add_test(test_random test_random)

