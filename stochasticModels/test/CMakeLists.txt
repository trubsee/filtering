enable_testing()

include (GoogleTest)

add_executable(stochasticModelsTest
    randomWalkTest.cpp
)

target_link_libraries(stochasticModelsTest
    alglib
    stochasticModels
    gtest_main
)

add_test(NAME testStochasticModels COMMAND stochasticModelsTest)
