add_library(commonLib
    assert.hpp assert.cpp
    constants.hpp
    randomSample.hpp
    statistics.hpp
    types.hpp
    utils.hpp
)

add_subdirectory(test)

target_link_libraries(commonLib
    Boost::boost
    Eigen3::Eigen
)
