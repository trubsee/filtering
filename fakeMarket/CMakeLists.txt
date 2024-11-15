add_library(fakeMarketLib
    events.hpp
    market.hpp market.cpp
    orderBook.hpp orderBook.cpp
)

add_subdirectory(test)

target_link_libraries(fakeMarketLib
    Boost::boost
    commonLib
)
