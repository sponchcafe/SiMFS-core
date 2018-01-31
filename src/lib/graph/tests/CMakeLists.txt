# Test graph
add_executable(graph_test graph_test.cpp)
add_test(graph_test graph_test)
target_link_libraries(graph_test gtest_main)
target_link_libraries(graph_test graph)
