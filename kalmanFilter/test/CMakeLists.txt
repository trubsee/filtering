enable_testing()

include (GoogleTest)

add_executable(kalmanFilterTest
    basicKFTest.cpp
)

target_link_libraries(kalmanFilterTest
    kalmanFilter
    gtest_main
)

gtest_discover_tests(kalmanFilterTest
    TEST_SUFFIX .bakTest
)

add_test(NAME testKF COMMAND kalmanFilterTest)
