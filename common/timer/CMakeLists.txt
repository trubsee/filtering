add_library(timer
    dummyClock.hpp dummyClock.cpp
    singleThreadedTimerManager.hpp
    timer.hpp timer.cpp
)

add_subdirectory(test)
