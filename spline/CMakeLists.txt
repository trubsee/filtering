add_library(splineLib
    bspline.hpp bspline.ipp bspline.cpp
)

add_subdirectory(test)

target_link_libraries(splineLib
    commonLib
    Eigen3::Eigen
)
