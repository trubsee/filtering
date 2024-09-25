enable_testing()

add_library(kalmanFilter
    basicKF.hpp basicKF.cpp
)

add_subdirectory(test)

target_link_libraries(kalmanFilter Eigen3::Eigen
)
