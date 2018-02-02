# graph library
FILE(GLOB graph_src src/*.cpp)
add_library(graph ${graph_src})
target_include_directories(graph PUBLIC ./include)
target_link_libraries(graph random)

# Tests
add_subdirectory(tests)
