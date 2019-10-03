set(TARGET_NAME test_img)

add_library(${TARGET_NAME} OBJECT test_img.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC sim_img sim_component catch2)

