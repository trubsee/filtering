#include <algorithm>
#include <functional>
#include <cmath>

#include "common/assert.hpp"
#include "fakeMarket/orderBook.hpp"

namespace FakeMarket {

bool OrderBook::QuoteDelete(const SubmitQuoteDelete& quoteDelete)
{
    const auto id = Common::MakeHashId(
            quoteDelete.clientId, 
            quoteDelete.productId, 
            quoteDelete.quoteId
    );

    const auto it = mQuotes.find(id);
    if (it == mQuotes.end())
        return false;

    const auto [side, price, volume] = it->second;
    QuoteVec& quotes = side == Side::BUY ? mBidQuotes[price] : mAskQuotes[price];
    const auto quoteIt = std::find_if(
        quotes.begin(), 
        quotes.end(), 
        [&id](const auto& quote){ return quote.second == id; }
    );
    ASSERT(quoteIt != quotes.end());
    quotes.erase(quoteIt);
    mQuotes.erase(it);
    return true;
}

bool OrderBook::FAK(const SubmitFAK& fak)
{
    if (std::fmod(fak.price, mTickSize) != 0)
        return false;

    if (fak.side == Side::BUY)
    {
        if (fak.price < mAskQuotes.begin()->first)
            return true;
        CrossBook(fak.price, fak.volume, mAskQuotes);
    }
    else
    {
        if (fak.price < mBidQuotes.begin()->first)
            return true;
        CrossBook(fak.price, fak.volume, mBidQuotes);
    }
    return true;
}

template<typename Compare>
Volume OrderBook::CrossBook(
    Price price, 
    Volume volume, 
    std::map<Price, QuoteVec, Compare>& allQuotes) 
{
    const auto fillVolume = [&volume](auto& quote){
        if (volume < quote.first)
        {
            quote.first -= volume;
            return true;
        }
        volume -= quote.first;
        return false;
    };
    const auto cleanUp = [this](const auto& quote) {
        auto numErased = mQuotes.erase(quote.second);
        ASSERT(numErased == 1);
    };

    while (!allQuotes.empty() && 
        Compare()(allQuotes.begin()->first, price))
    {
        auto quotes = allQuotes.begin()->second;
        auto it = std::find_if(quotes.begin(), quotes.end(), fillVolume);
        
        // Delete our records in unordered_map
        std::for_each(quotes.begin(), it, cleanUp); 
        if (it != quotes.end())
        {
            quotes.erase(quotes.begin(), it);
            return Volume{0};
        }
        // Delete price volume entirely
        quotes.erase(quotes.begin());
    }
    return volume;
}

}
