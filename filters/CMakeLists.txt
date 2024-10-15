enable_testing()

add_library(filters
    kalmanFilter.hpp kalmanFilter.ipp
    particleFilter.hpp particleFilter.cpp
)

add_subdirectory(test)

target_link_libraries(filters 
    alglib
    Eigen3::Eigen
    common
    stochasticModels
)
