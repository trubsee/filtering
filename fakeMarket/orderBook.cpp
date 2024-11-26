#include <algorithm>
#include <functional>
#include <numeric>

#include "fakeMarket/orderBook.hpp"

namespace FakeMarket {

bool OrderBook::QuoteDelete(const SubmitQuoteDelete& qd)
{
    const auto id = Common::MakeHashId(
        qd.clientId, qd.productId, qd.quoteId
    );

    const auto it = mQuotes.find(id);
    if (it == mQuotes.end())
        return false;

    const auto [side, price, volume] = it->second;
    auto& quotes = GetPriceLevel(side, price);
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

bool OrderBook::QuoteUpdate(const SubmitQuoteUpdate& qu)
{
    if (!CheckValidPrice(qu.price))
        return false;
    
    const auto id = Common::MakeHashId(
        qu.clientId, qu.productId, qu.quoteId
    );
    const auto it = mQuotes.find(id);
    if (it != mQuotes.end() && std::get<Side>(it->second) != qu.side)
        return false;

    Volume endVolume = qu.volume;
    if (qu.side == Side::BUY && qu.price > mAskQuotes.begin()->first)
        endVolume = CrossBook(qu.price, qu.volume, mAskQuotes);
    else if (qu.side == Side::SELL && qu.price < mBidQuotes.begin()->first)
        endVolume = CrossBook(qu.price, qu.volume, mBidQuotes);

    if (endVolume == Volume{0})
    {
        if (it != mQuotes.end())
            mQuotes.erase(it);
        return true;
    }
    
    if (it != mQuotes.end())
    {
        auto& [_, oldPrice, oldVolume] = it->second; 
        oldVolume = endVolume;
        oldPrice = qu.price;

        auto& oldLevel = GetPriceLevel(qu.side, qu.price);
        auto qIt = std::find_if(
            oldLevel.begin(), 
            oldLevel.end(), 
            [&id](const auto& qi){ return id == qi.second; }
        );
        ASSERT (qIt != oldLevel.end());
        // Policy is to lose queue prio
        oldLevel.erase(qIt);
    }
    else
    {
        mQuotes.emplace(
            id, SingleQuoteDetails{qu.side, qu.price, qu.volume}
        );
    }
    auto& newLevel = GetPriceLevel(qu.side, qu.price);
    newLevel.emplace_back(QuoteInfo{endVolume, id});
    return true;
}

bool OrderBook::FAK(const SubmitFAK& fak)
{
    if (!CheckValidPrice(fak.price))
        return false;

    if (fak.side == Side::BUY)
    {
        if (fak.price < mAskQuotes.begin()->first)
            return true;
        CrossBook(fak.price, fak.volume, mAskQuotes);
    }
    else
    {
        if (fak.price > mBidQuotes.begin()->first)
            return true;
        CrossBook(fak.price, fak.volume, mBidQuotes);
    }
    return true;
}

TOB OrderBook::GetTopOfBook() const
{
    const auto [bidPrice, bidVolumes] = *mBidQuotes.begin();
    const auto bidVolume = std::accumulate(
       bidVolumes.begin(),
       bidVolumes.end(),
       Volume{0},
       [](Volume volume, const auto& quote) { return quote.first + volume; }
    );
    const auto [askPrice, askVolumes] = *mAskQuotes.begin();
    const auto askVolume = std::accumulate(
       askVolumes.begin(),
       askVolumes.end(),
       Volume{0},
       [](Volume volume, const auto& quote) { return quote.first + volume; }
    );
    return TOB{ bidPrice, bidVolume, askPrice, askVolume };
}

void OrderBook::SetBook(BidQuotesMap bidQuotes, AskQuotesMap askQuotes)
{
    ASSERT(mQuotes.empty());
    mBidQuotes = bidQuotes;
    for (const auto& priceLevel: bidQuotes)
    {
        const auto price = priceLevel.first;
        for (const auto& quote: priceLevel.second)
        {
            auto inserted = mQuotes.emplace(
                quote.second, 
                SingleQuoteDetails{Side::BUY, price, quote.first}
            );
            ASSERT(inserted.second);
        }
    }
    mAskQuotes = askQuotes;
    for (const auto& priceLevel: askQuotes)
    {
        const auto price = priceLevel.first;
        for (const auto& quote: priceLevel.second)
        {
            auto inserted = mQuotes.emplace(
                quote.second, 
                SingleQuoteDetails{Side::SELL, price, quote.first}
            );
            ASSERT(inserted.second);
        }
    }
}

template<typename Compare>
Volume OrderBook::CrossBook(
    Price price, 
    Volume volume, 
    std::map<Price, QuoteVec, Compare>& priceLevels) 
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

    while (!priceLevels.empty() && 
        !Compare()(price, priceLevels.begin()->first))
    {
        auto& quotes = priceLevels.begin()->second;
        auto it = std::find_if(quotes.begin(), quotes.end(), fillVolume);
        
        // Delete our records in unordered_map
        std::for_each(quotes.begin(), it, cleanUp); 
        if (it != quotes.end())
        {
            quotes.erase(quotes.begin(), it);
            const auto infoIt = mQuotes.find(it->second);
            ASSERT(infoIt != mQuotes.end());
            std::get<Volume>(infoIt->second) -= volume;
            return Volume{0};
        }
        
        // Delete price volume entirely
        priceLevels.erase(priceLevels.begin());
    }
    return volume;
}

}
