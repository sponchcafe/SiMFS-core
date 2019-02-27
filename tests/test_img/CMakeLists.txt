set(TARGET_NAME test_img)

add_executable(${TARGET_NAME} test_main.cpp test_img.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_img sim_component catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
