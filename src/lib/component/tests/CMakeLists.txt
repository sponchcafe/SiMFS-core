# Test io
add_executable(component_test component_test.cpp)
add_test(component_test component_test)
target_link_libraries(component_test gtest_main sim_component)

