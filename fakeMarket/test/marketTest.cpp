#include "fakeMarket/events.hpp"
#include "fakeMarket/market.hpp"
#include "gtest/gtest.h"
#include "timer/singleThreadedTimerManager.hpp"

namespace FakeMarket {

TEST(MaketTest, RegisterOrders) {
    Market market(0.1);
    auto client = market.AddClient();
    Timer::SingleThreadedTimerManager timerManager;

    bool isOpen{true};
    auto timerId = timerManager.CreateTimer(std::chrono::seconds(1),
                                            [&isOpen]() { isOpen = false; });

    SubmitQuoteUpdate qu{MsgNumber{1},
                         ClientId{client.GetClientId()},
                         ProductId{1},
                         Side::SELL,
                         Price{100.},
                         Volume{10}};
    client.RegisterQuoteUpdate(qu);
    SubmitFAK fak{MsgNumber{2},
                  ClientId{client.GetClientId()},
                  ProductId{1},
                  Side::BUY,
                  Price{100.},
                  Volume{1}};
    client.RegisterFAK(fak);

    while (isOpen) {
        market.Tick();
        timerManager.Update();
    }

    auto fill = client.GetFill();
    EXPECT_TRUE(fill);
}

}  // namespace FakeMarket
