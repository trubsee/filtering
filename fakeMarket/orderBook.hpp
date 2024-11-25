#pragma once
#include <map>
#include <unordered_map>
#include <vector>

#include "common/assert.hpp"
#include "common/types.hpp"
#include "common/utils.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

class OrderBook
{
    using SingleQuoteDetails = std::tuple<Side, Price, Volume>;
    using QuoteInfo = std::pair<Volume, Common::HashId>;
    using QuoteVec = std::vector<QuoteInfo>;
public:
    OrderBook(double tickSize) : mTickSize(tickSize) {}
   
    bool QuoteDelete(const SubmitQuoteDelete&);

    bool QuoteUpdate(const SubmitQuoteUpdate&);

    bool FAK(const SubmitFAK&);

    TOB GetTopOfBook() const;

private:
    template<typename Compare> 
    Volume CrossBook(Price, Volume, std::map<Price, QuoteVec, Compare>&);

    QuoteVec& GetPriceLevel(Side side, Price price)
    {
        if (side == Side::BUY)
        {
            auto it = mBidQuotes.find(price);
            ASSERT(it != mBidQuotes.end());
            return it->second;
        }
        else
        {
            auto it = mAskQuotes.find(price);
            ASSERT(it != mAskQuotes.end());
            return it->second;
        }
    }

    double mTickSize;
    std::unordered_map<Common::HashId, SingleQuoteDetails> mQuotes;
    std::map<Price, QuoteVec, std::greater<Price>> mBidQuotes;
    std::map<Price, QuoteVec> mAskQuotes;
};

}
