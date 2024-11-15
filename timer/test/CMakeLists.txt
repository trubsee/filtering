enable_testing()

include(GoogleTest)

add_executable(timerTest
    repeatingTimerTest.cpp
    singleThreadedTimerManagerTest.cpp
)

target_link_libraries(timerTest
    timerLib
    gtest_main
)

add_test(NAME testTimer COMMAND timerTest)
