set(TARGET_NAME test_component)

add_library(${TARGET_NAME} OBJECT test_buffer.cpp test_component.cpp) 
target_link_libraries(${TARGET_NAME} PUBLIC sim_component sim_util catch2)
