#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "common/assert.hpp"
#include "common/types.hpp"
#include "common/utils.hpp"
#include "fakeMarket/clientUpdater.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

namespace Test {
class OrderBookTestFixture;
}

class OrderBook {
   public:
    using SingleQuoteDetails = std::tuple<Side, Price, Volume>;
    using QuoteInfo = std::pair<Volume, Common::HashId>;
    using QuoteVec = std::vector<QuoteInfo>;
    using BidQuotesMap = std::map<Price, QuoteVec, std::greater<Price>>;
    using AskQuotesMap = std::map<Price, QuoteVec>;

    OrderBook(ClientUpdater& clientUpdater, double tickSize)
        : mClientUpdater{clientUpdater}, mTickSize{tickSize} {}

    void QuoteDelete(const SubmitQuoteDelete&);

    void QuoteUpdate(const SubmitQuoteUpdate&);

    void FAK(const SubmitFAK&);

    TOB GetTopOfBook() const;

    void SetBook(BidQuotesMap, AskQuotesMap);

   private:
    template <typename Compare>
    Volume CrossBook(ClientId, QuoteId, Price, Volume, std::map<Price, QuoteVec, Compare>&);

    bool CheckValidPrice(Price price) {
        return std::abs(price - std::round(price / mTickSize) * mTickSize) <
               1e-9;
    }

    template <bool isBuy>
    QuoteVec& GetPriceLevel(Price price);
    
    template <bool isBuy>
    void DeletePriceLevel(Price price);
    
    void FillQuote(Volume removeVolume, QuoteInfo& quote)
    {
        const auto [cId, pId, qId] = Common::ExtractHashId(quote.second);
        const auto it = mQuotes.find(quote.second);
        ASSERT(it != mQuotes.end());
        auto& [side, price, volume] = it->second;
        if (removeVolume == Volume{0}) {
            mQuotes.erase(it);
            mClientUpdater.SendFillPrivate(cId, {qId, price, volume});
        } else {
            volume -= removeVolume;
            mClientUpdater.SendFillPrivate(cId, {qId, price, removeVolume});
        }
    }

    ClientUpdater& mClientUpdater;
    double mTickSize;

    std::unordered_map<Common::HashId, SingleQuoteDetails> mQuotes;
    BidQuotesMap mBidQuotes;
    AskQuotesMap mAskQuotes;

    friend class Test::OrderBookTestFixture;
};

}  // namespace FakeMarket
