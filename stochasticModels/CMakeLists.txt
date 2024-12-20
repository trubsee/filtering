enable_testing()

add_library(stochasticModelsLib
    linearFactoryMethods.hpp
    linearGaussian.hpp
    linearGaussianTraits.hpp
    stochasticVolatility.hpp stochasticVolatility.cpp
)

add_subdirectory(test)

target_link_libraries(stochasticModelsLib
    Eigen3::Eigen
    commonLib
)
