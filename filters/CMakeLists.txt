enable_testing()

add_library(filtersLib
    kalmanFilter.hpp
    particleFilter.hpp particleFilter.cpp
    stateSpaceModelTraits.hpp
    stochasticEKF.hpp stochasticEKF.ipp
)

add_subdirectory(test)
add_subdirectory(benchmark)

target_link_libraries(filtersLib 
    alglib
    Eigen3::Eigen
    commonLib
    stochasticModelsLib
)
