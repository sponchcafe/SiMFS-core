set(TARGET_NAME test_focus)

add_executable(${TARGET_NAME} test_main.cpp test_gauss.cpp test_alpha.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_focus sim_util catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
