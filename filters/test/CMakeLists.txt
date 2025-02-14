enable_testing()

include (GoogleTest)

add_executable(filtersTest
    deterministicEKFTest.cpp
    kalmanFilterTest.cpp
    particleFilterTest.cpp
    stochasticEKFTest.cpp
)

target_link_libraries(filtersTest
    alglib
    filtersLib
    gtest_main
)

add_test(NAME testFilters COMMAND filtersTest)
