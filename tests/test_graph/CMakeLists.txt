set(TARGET_NAME test_graph)

add_executable(${TARGET_NAME} test_main.cpp test_graph.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_graph catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
