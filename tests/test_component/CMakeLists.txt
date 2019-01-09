set(TARGET_NAME test_component)

add_executable(${TARGET_NAME} test_main.cpp test_buffer.cpp test_component.cpp) 
target_link_libraries(${TARGET_NAME} PUBLIC sim_component sim_util catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
