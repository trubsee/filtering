#include <iostream>

#include "common/eventDispatcher.hpp"
#include "common/utils.hpp"
#include "fakeMarket/basicClient.hpp"
#include "fakeMarket/clientUpdater.hpp"
#include "fakeMarket/events.hpp"
#include "fakeMarket/orderBook.hpp"
#include "gtest/gtest.h"

namespace FakeMarket::Test {

class OrderBookTestFixture : public testing::Test {
   public:
    void SetUp() override {
        cu.ConnectClient(client1);
        cu.ConnectClient(client2);
        cu.ConnectClient(client3);

        OrderBook::BidQuotesMap bidQuotes;
        bidQuotes.emplace(
            Price{1},
            OrderBook::QuoteVec{
                {Volume{5},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{1})},
                {Volume{2},
                 Common::MakeHashId(client2.GetClientId(), pId, QuoteId{1})},
                {Volume{3},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{2})}});
        bidQuotes.emplace(
            Price{0.95},
            OrderBook::QuoteVec{
                {Volume{8},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{3})},
                {Volume{4},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{8})}});
        bidQuotes.emplace(
            Price{0.9},
            OrderBook::QuoteVec{
                {Volume{8},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{4})}});

        OrderBook::AskQuotesMap askQuotes;
        askQuotes.emplace(
            Price{1.1},
            OrderBook::QuoteVec{
                {Volume{4},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{5})},
                {Volume{4},
                 Common::MakeHashId(client2.GetClientId(), pId, QuoteId{2})},
            });
        askQuotes.emplace(
            Price{1.15},
            OrderBook::QuoteVec{
                {Volume{4},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{6})}});
        askQuotes.emplace(
            Price{1.2},
            OrderBook::QuoteVec{
                {Volume{20},
                 Common::MakeHashId(client1.GetClientId(), pId, QuoteId{7})}});

        ob.SetBook(bidQuotes, askQuotes);
    }

    bool CheckQuoteExists(Common::HashId id) {
        return ob.mQuotes.find(id) != ob.mQuotes.end();
    }

    OrderBook::SingleQuoteDetails GetQuote(Common::HashId id) {
        return ob.mQuotes[id];
    }

    Common::EventDispatcher ed;
    ClientUpdater cu{ed};
    OrderBook ob{cu, 0.05};
    ProductId pId{1};
    BasicClient client1;
    BasicClient client2;
    BasicClient client3;
};

TEST_F(OrderBookTestFixture, TestFAK) {
    auto expectedTob = ob.GetTopOfBook();

    // Miss
    SubmitFAK fak{MsgNumber{1},
                  client1.GetClientId(),
                  pId,
                  Side::SELL,
                  Price{1.05},
                  Volume{1}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());

    fak = SubmitFAK{MsgNumber{2},
                    client1.GetClientId(),
                    pId,
                    Side::BUY,
                    Price{1.05},
                    Volume{1}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());

    // Bad Price
    fak = SubmitFAK{MsgNumber{3},
                    client1.GetClientId(),
                    pId,
                    Side::SELL,
                    Price{0.98},
                    Volume{1}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::PRICE_NA_TICK);

    fak = SubmitFAK{MsgNumber{4},
                    client1.GetClientId(),
                    pId,
                    Side::BUY,
                    Price{1.28},
                    Volume{1}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::PRICE_NA_TICK);

    // Hit TOB first quote
    fak = SubmitFAK{MsgNumber{5},
                    client1.GetClientId(),
                    pId,
                    Side::SELL,
                    Price{1},
                    Volume{1}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    expectedTob.bidVolume -= 1;
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());
    auto quote =
        GetQuote(Common::MakeHashId(client1.GetClientId(), pId, QuoteId{1}));
    EXPECT_EQ(std::get<Volume>(quote), Volume{4});

    fak = SubmitFAK{MsgNumber{6},
                    client1.GetClientId(),
                    pId,
                    Side::BUY,
                    Price{1.1},
                    Volume{1}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    expectedTob.askVolume -= 1;
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());
    quote =
        GetQuote(Common::MakeHashId(client1.GetClientId(), pId, QuoteId{5}));
    EXPECT_EQ(std::get<Volume>(quote), Volume{3});

    // Clear level
    fak = SubmitFAK{MsgNumber{7},
                    client1.GetClientId(),
                    pId,
                    Side::SELL,
                    Price{1},
                    Volume{12}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    expectedTob.bidVolume = 12;
    expectedTob.bidPrice = 0.95;
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client1.GetClientId(), pId, QuoteId{1})));
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client2.GetClientId(), pId, QuoteId{1})));
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client1.GetClientId(), pId, QuoteId{2})));

    // Trade on two levels
    fak = SubmitFAK{MsgNumber{8},
                    client1.GetClientId(),
                    pId,
                    Side::BUY,
                    Price{1.2},
                    Volume{10}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    expectedTob.askVolume = 1;
    expectedTob.askPrice = 1.15;
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client1.GetClientId(), pId, QuoteId{1})));
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client2.GetClientId(), pId, QuoteId{1})));
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client1.GetClientId(), pId, QuoteId{2})));

    // Fill one quote
    fak = SubmitFAK{MsgNumber{9},
                    client1.GetClientId(),
                    pId,
                    Side::SELL,
                    Price{0.95},
                    Volume{8}};
    ob.FAK(fak);
    EXPECT_EQ(client1.GetResponse()->result, Result::OK);
    expectedTob.bidVolume -= 8;
    EXPECT_EQ(expectedTob, ob.GetTopOfBook());
    EXPECT_FALSE(CheckQuoteExists(
        Common::MakeHashId(client1.GetClientId(), pId, QuoteId{3})));
    quote =
        GetQuote(Common::MakeHashId(client1.GetClientId(), pId, QuoteId{8}));
    EXPECT_EQ(std::get<Volume>(quote), Volume{4});
}

}  // namespace FakeMarket::Test
