# Test io
add_executable(sim_component_test component_test.cpp)
add_test(sim_component_test component_test.cpp)
target_link_libraries(sim_component_test gtest_main sim_component)

