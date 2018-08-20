add_library(sim_util src/random.cpp)
target_include_directories(sim_util PUBLIC ./include)

# Test excecutable
add_executable(sim_random_test tests/random_test.cpp)
target_link_libraries(sim_random_test sim_util)
target_link_libraries(sim_random_test gtest_main)
add_test(sim_random_test sim_random_test)

