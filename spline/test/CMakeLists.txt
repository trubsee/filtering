enable_testing()

include (GoogleTest)

add_executable(splineTest
    bsplineTest.cpp
)

target_link_libraries(splineTest
    splineLib
    gtest_main
)

add_test(NAME testSpline COMMAND splineTest)
