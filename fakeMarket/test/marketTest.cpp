#include "gtest/gtest.h"

#include "fakeMarket/events.hpp"
#include "fakeMarket/market.hpp"

namespace FakeMarket {

TEST(MaketTest, RegisterOrders)
{
    Market market(15, 0.1);
   
    SubmitFAK fak{
        ClientId{0},
        ProductId{0},
        Side::BUY,
        Price{100.},
        Volume{10}
    };
    market.RegisterFAK(fak);
    market.Run();
}

}
