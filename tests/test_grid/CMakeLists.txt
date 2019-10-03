set(TARGET_NAME test_grid)

add_library(${TARGET_NAME} OBJECT test_grid.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_grid sim_util catch2)
