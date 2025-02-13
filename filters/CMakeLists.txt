enable_testing()

add_library(filtersLib
    kalmanFilter.hpp
    particleFilter.hpp particleFilter.cpp
    pythonFilter.cpp
    python/kf.cpp
    stateSpaceModelTraits.hpp
    stochasticEKF.hpp stochasticEKF.ipp
)

find_package(Python3 REQUIRED Interpreter Development)
add_subdirectory(test)
add_subdirectory(benchmark)

target_link_libraries(filtersLib 
    alglib
    Eigen3::Eigen
    commonLib
    pybind11::module
    Python3::Python
    stochasticModelsLib
)


include_directories(${eigen_SOURCE_DIR})
pybind11_add_module(fast_filters pythonFilter.cpp)

