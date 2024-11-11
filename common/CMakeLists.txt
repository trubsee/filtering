add_library(common
    assert.hpp assert.cpp
    constants.hpp
    randomSample.hpp
    statistics.hpp
)

add_subdirectory(timer)

target_link_libraries(common
    Eigen3::Eigen
)
