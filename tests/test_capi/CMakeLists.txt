set(TARGET_NAME test_diff_capi)

add_executable(${TARGET_NAME} test_diff_capi.cpp ../test_main.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_dif catch2)
add_test(${TARGET_NAME} ${TARGET_NAME})
