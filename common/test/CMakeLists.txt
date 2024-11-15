enable_testing()

include (GoogleTest)

add_executable(commonTest
    eventDispatcherTest.cpp
    utilsTest.cpp
)

target_link_libraries(commonTest
    commonLib
    gtest_main
)

add_test(NAME testCommon COMMAND commonTest)
