set(TARGET_NAME test_shape)

add_library(${TARGET_NAME} OBJECT 
    test_factory.cpp
    test_XYZGauss.cpp 
    test_XYGaussZExp.cpp 
    test_gaussBeam.cpp
)

target_link_libraries(${TARGET_NAME} PUBLIC sim_shape sim_util catch2)
