add_library(commonLib
    assert.hpp assert.cpp
    constants.hpp
    mmapReader.hpp
    mmapWriter.hpp
    randomSample.hpp
    statistics.hpp
    types.hpp types.cpp
    utils.hpp
)

add_subdirectory(test)

target_link_libraries(commonLib
    Boost::boost
    Eigen3::Eigen
    pthread
    ${RT_LIBRARY}
)
