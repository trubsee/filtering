#pragma once
#include <map>
#include <unordered_map>
#include <vector>

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

    bool FAK(const SubmitFAK&);

private:
    template<typename Compare> 
    Volume CrossBook(Price, Volume, std::map<Price, QuoteVec, Compare>&);

    double mTickSize;
    std::unordered_map<Common::HashId, SingleQuoteDetails> mQuotes;
    std::map<Price, QuoteVec, std::greater<Price>> mBidQuotes;
    std::map<Price, QuoteVec> mAskQuotes;
};

}
