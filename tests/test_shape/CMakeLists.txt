set(TARGET_NAME test_shape)

add_executable(${TARGET_NAME} 
    test_main.cpp 
    test_factory.cpp
    test_XYZGauss.cpp 
    test_XYGaussZExp.cpp 
    test_gaussBeam.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC sim_shape sim_util catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
