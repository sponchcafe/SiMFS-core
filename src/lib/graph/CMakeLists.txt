FILE(GLOB graph_src src/*.cpp)
add_library(graph ${graph_src})
target_include_directories(graph PUBLIC ./include)
target_link_libraries(graph random)

# Test excecutable
add_executable(test_graph test/test_graph.cpp)
target_link_libraries(test_graph sim)
target_link_libraries(test_graph gtest_main)
target_link_libraries(test_graph graph)
add_test(test_graph test_graph)

