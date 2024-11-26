add_library(fakeMarketLib
    events.hpp
    client.hpp client.cpp
    clientUpdater.hpp
    IClient.hpp
    market.hpp market.cpp
    orderBook.hpp orderBook.cpp
)

add_subdirectory(test)

target_link_libraries(fakeMarketLib
    Boost::boost
    commonLib
    gtest_main
)
