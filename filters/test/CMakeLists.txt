enable_testing()

include (GoogleTest)

add_executable(filtersTest
    basicKFTest.cpp
)

target_link_libraries(filtersTest
    filters
    gtest_main
)

add_test(NAME testKF COMMAND filtersTest)
