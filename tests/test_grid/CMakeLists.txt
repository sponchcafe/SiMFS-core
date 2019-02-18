set(TARGET_NAME test_grid)

add_executable(${TARGET_NAME} test_main.cpp test_grid.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_grid sim_util catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
