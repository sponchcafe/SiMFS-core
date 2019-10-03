set(TARGET_NAME test_graph)

add_library(${TARGET_NAME} OBJECT test_graph.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_graph catch2)
