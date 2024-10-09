enable_testing()

add_library(stochasticModels
    randomWalk.hpp randomWalk.cpp
)

add_subdirectory(test)

target_link_libraries(stochasticModels
    Eigen3::Eigen
    common
)
