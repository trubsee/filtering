#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <variant>

#include "common/types.hpp"
#include "fakeMarket/events.hpp"
#include "fakeMarket/orderBook.hpp"

namespace FakeMarket {

class Market
{
    using MarketOrder = std::variant<
        SubmitFAK, 
        SubmitQuoteUpdate, 
        SubmitQuoteDelete
    >;

public:
    Market(unsigned lifetime, double tickSize);

    void Run();

    ClientId AddClient(); 

    void RegisterFAK(const SubmitFAK& fak) { AddToOrders(fak); }
    
    void RegisterQuoteUpdate(const SubmitQuoteUpdate& quoteUpdate) { AddToOrders(quoteUpdate); }
    
    void RegisterQuoteDelete(const SubmitQuoteDelete& quoteDelete) { AddToOrders(quoteDelete); }

private:
    void AddToOrders(const MarketOrder&);

    void ProcessOrder(const MarketOrder&);

    const unsigned mLifetime;
    ClientId mLastClientId;
    
    // consider using tbb_concurrent_queue
    std::mutex mOrderMutex;
    std::condition_variable mOrderCV; 
    std::queue<MarketOrder> mOrders;

    std::unordered_map<std::uint8_t, OrderBook> mOrderBooks;
};

}
