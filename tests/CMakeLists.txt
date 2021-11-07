add_subdirectory(./catch2/)

add_subdirectory(./test_component/)
add_subdirectory(./test_shape/)
add_subdirectory(./test_utilities/)
add_subdirectory(./test_graph/)
add_subdirectory(./test_grid/)
add_subdirectory(./test_mix/)
add_subdirectory(./test_img/)
add_subdirectory(./test_capi/)


add_executable(unittests test_main.cpp)
target_link_libraries(unittests 
    PUBLIC 
        test_component
        test_graph
        #test_grid
        test_img
        test_mix
        test_shape
        test_utilities
)

add_test(NAME unittests COMMAND unittests -r compact -s)
