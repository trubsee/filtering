enable_testing()

add_library(filtersLib
    deterministicEKF.hpp deterministicEKF.ipp
    kalmanFilter.hpp
    particleFilter.hpp particleFilter.cpp
    pythonFilter.cpp
    python/kf.cpp
    python/dekf.cpp
    python/sekf.cpp
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

