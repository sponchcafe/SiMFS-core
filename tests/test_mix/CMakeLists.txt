set(TARGET_NAME test_mix)

add_executable(${TARGET_NAME} test_main.cpp test_mix.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_mix sim_component catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
