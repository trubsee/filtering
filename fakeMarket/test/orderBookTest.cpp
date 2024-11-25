#include "gtest/gtest.h"

#include "common/utils.hpp"

#include "fakeMarket/events.hpp"
#include "fakeMarket/orderBook.hpp"

namespace FakeMarket::Test {

class OrderBookTestFixture : public testing::Test
{
public:
    void SetUp() override
    {
        const auto hid1 = Common::MakeHashId(ClientId{1}, ProductId{1}, QuoteId{1});
        mOrderBook.mQuotes.emplace(hid1, OrderBook::SingleQuoteDetails{Side::BUY, 1, 10}); 
    }

    void TearDown() override {}
    OrderBook mOrderBook{0.5};
};

TEST_F(OrderBookTestFixture, TestFAK)
{
}

}
