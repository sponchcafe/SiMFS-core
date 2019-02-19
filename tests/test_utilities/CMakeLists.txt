set(TARGET_NAME test_utilities)

add_executable(${TARGET_NAME} test_main.cpp test_env.cpp test_random.cpp test_baseIO.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_util sim_baseIO catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
