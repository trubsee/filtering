add_library(common
    assert.hpp assert.cpp
    constants.hpp
    randomSample.hpp
    statistics.hpp
)

target_link_libraries(common
    Eigen3::Eigen
)
