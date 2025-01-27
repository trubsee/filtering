#pragma once
#include <iostream>
#include <unordered_map>

#include "common/eventDispatcher.hpp"
#include "common/types.hpp"
#include "concurrentqueue.h"
#include "fakeMarket/basicClient.hpp"
#include "fakeMarket/clientUpdater.hpp"
#include "fakeMarket/events.hpp"
#include "fakeMarket/orderBook.hpp"

namespace FakeMarket {

class Market {
   public:
    Market(double tickSize);

    void Tick();

    BasicClient AddClient();

   private:
    void AddToOrders(const MarketOrder&);

    void ProcessOrder(const MarketOrder&);

    Common::EventDispatcher mEventDispatcher;
    ClientUpdater mClientUpdater;

    moodycamel::ConcurrentQueue<MarketOrder> mOrders;

    std::unordered_map<std::uint8_t, OrderBook> mOrderBooks;
};

}  // namespace FakeMarket
