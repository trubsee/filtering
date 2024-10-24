enable_testing()

include (GoogleTest)

add_executable(filtersTest
    kalmanFilterTest.cpp
    particleFilterTest.cpp
    stochasticEKFTest.cpp
)

target_link_libraries(filtersTest
    alglib
    filters
    gtest_main
)

add_test(NAME testFilters COMMAND filtersTest)
