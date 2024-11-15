enable_testing()

add_library(filtersLib
    kalmanFilter.hpp kalmanFilter.cpp
    particleFilter.hpp particleFilter.cpp
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
