enable_testing()

add_library(filters
    kalmanFilter.hpp kalmanFilter.cpp
    particleFilter.hpp particleFilter.cpp
    stochasticEKF.hpp stochasticEKF.ipp
)

add_subdirectory(test)

target_link_libraries(filters 
    alglib
    Eigen3::Eigen
    common
    stochasticModels
)
