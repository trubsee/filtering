enable_testing()

add_library(filters
    basicKF.hpp basicKF.cpp
    particleFilter.hpp particleFilter.cpp
)

add_subdirectory(test)

target_link_libraries(filters 
    alglib
    Eigen3::Eigen
    common
)
