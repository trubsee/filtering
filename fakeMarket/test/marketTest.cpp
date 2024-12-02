#include "fakeMarket/events.hpp"
#include "fakeMarket/market.hpp"
#include "gtest/gtest.h"

namespace FakeMarket {

TEST(MaketTest, RegisterOrders) {
    Market market(15, 0.1);

    SubmitFAK fak{MsgNumber{1},
                  ClientId{0},
                  ProductId{0},
                  Side::BUY,
                  Price{100.},
                  Volume{10}};
    market.RegisterFAK(fak);
    market.Run();
}

}  // namespace FakeMarket
