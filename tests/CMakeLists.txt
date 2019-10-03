add_subdirectory(./catch2/)

add_subdirectory(./test_component/)
add_subdirectory(./test_shape/)
add_subdirectory(./test_utilities/)
add_subdirectory(./test_graph/)
add_subdirectory(./test_grid/)
add_subdirectory(./test_mix/)
add_subdirectory(./test_img/)


add_executable(unittests test_main.cpp)
target_link_libraries(unittests 
    PUBLIC 
        test_component
        test_graph
        test_grid
        test_img
        test_mix
        test_shape
        test_utilities
)

add_test(NAME unittests COMMAND unittests -r compact -s)

add_custom_target(gcov
    COMMAND ${CMAKE_MAKE_PROGRAM} test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

add_custom_command(TARGET gcov
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/gcov.sh ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

add_dependencies(gcov unittests)
