enable_testing()

add_library(stochasticModels
    linearFactoryMethods.hpp
    linearGaussian.hpp linearGaussian.cpp
    stochasticVolatility.hpp stochasticVolatility.cpp
)

add_subdirectory(test)

target_link_libraries(stochasticModels
    Eigen3::Eigen
    common
)
