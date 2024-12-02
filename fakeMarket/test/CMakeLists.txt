enable_testing()

include (GoogleTest)

add_executable(fakeMarketTest
    #marketTest.cpp
    orderBookTest.cpp
)

target_link_libraries(fakeMarketTest
    fakeMarketLib
    gtest_main
)

add_test(NAME testFakeMarket COMMAND fakeMarketTest)
