#include "fakeMarket/orderBook.hpp"

#include <algorithm>
#include <functional>
#include <numeric>

namespace FakeMarket {

void OrderBook::QuoteDelete(const SubmitQuoteDelete& qd) {
    const auto id = Common::MakeHashId(qd.clientId, qd.productId, qd.quoteId);

    const auto it = mQuotes.find(id);
    if (it == mQuotes.end()) {
        mClientUpdater.SendResponse(qd.clientId,
                                    {qd.msgNumber, Result::QUOTE_DOESNT_EXIST});
        return;
    }

    mClientUpdater.SendResponse(qd.clientId, {qd.msgNumber, Result::OK});
    const auto [side, price, volume] = it->second;
    auto& quotes = side == Side::BUY ? 
        GetPriceLevel<true>(price) :
        GetPriceLevel<false>(price);
    const auto quoteIt =
        std::find_if(quotes.begin(), quotes.end(), [&id](const auto& quote) {
            return quote.second == id;
        });
    ASSERT(quoteIt != quotes.end());
    if (quotes.size() > 1)
        quotes.erase(quoteIt);
    else
    {
        side == Side::BUY ? 
            DeletePriceLevel<true>(price) :
            DeletePriceLevel<false>(price);
    }
    mQuotes.erase(it);
}

void OrderBook::QuoteUpdate(const SubmitQuoteUpdate& qu) {
    if (!CheckValidPrice(qu.price)) {
        mClientUpdater.SendResponse(qu.clientId,
                                    {qu.msgNumber, Result::PRICE_NA_TICK});
        return;
    }

    const auto id = Common::MakeHashId(qu.clientId, qu.productId, qu.quoteId);
    const auto it = mQuotes.find(id);
    if (it != mQuotes.end() && std::get<Side>(it->second) != qu.side) {
        mClientUpdater.SendResponse(
            qu.clientId, {qu.msgNumber, Result::CANNOT_AMEND_QUOTE_SIDE});
        return;
    }

    std::cout << (it == mQuotes.end()) << std::endl;
    if (it != mQuotes.end())
        QuoteDelete(SubmitQuoteDelete{qu.msgNumber, qu.clientId, qu.productId, qu.quoteId});
    else
        mClientUpdater.SendResponse(qu.clientId, {qu.msgNumber, Result::OK});

    Volume endVolume = qu.volume;
    if (qu.side == Side::BUY && qu.price >= mAskQuotes.begin()->first)
        endVolume = CrossBook(qu.clientId, qu.quoteId, qu.price, qu.volume, mAskQuotes);
    else if (qu.side == Side::SELL && qu.price <= mBidQuotes.begin()->first)
        endVolume = CrossBook(qu.clientId, qu.quoteId, qu.price, qu.volume, mBidQuotes);
    if (endVolume == Volume{0}) return;

    mQuotes.emplace(id, SingleQuoteDetails{qu.side, qu.price, qu.volume});
    auto& newLevel = qu.side == Side::BUY ? 
        GetPriceLevel<true>(qu.price) :
        GetPriceLevel<false>(qu.price);
    newLevel.emplace_back(QuoteInfo{endVolume, id});
}

void OrderBook::FAK(const SubmitFAK& fak) {
    if (!CheckValidPrice(fak.price)) {
        mClientUpdater.SendResponse(fak.clientId,
                                    {fak.msgNumber, Result::PRICE_NA_TICK});
        return;
    }

    mClientUpdater.SendResponse(fak.clientId, {fak.msgNumber, Result::OK});
    if (fak.side == Side::BUY && fak.price >= mAskQuotes.begin()->first) 
    {
        CrossBook(fak.clientId, QuoteId{0}, fak.price, fak.volume, mAskQuotes);
    } 
    else if (fak.side == Side::SELL && fak.price <= mBidQuotes.begin()->first) 
    {
        CrossBook(fak.clientId, QuoteId{0}, fak.price, fak.volume, mBidQuotes);
    }
}

TOB OrderBook::GetTopOfBook() const {
    const auto [bidPrice, bidVolumes] = *mBidQuotes.begin();
    const auto bidVolume = std::accumulate(
        bidVolumes.begin(),
        bidVolumes.end(),
        Volume{0},
        [](Volume volume, const auto& quote) { return quote.first + volume; });
    const auto [askPrice, askVolumes] = *mAskQuotes.begin();
    const auto askVolume = std::accumulate(
        askVolumes.begin(),
        askVolumes.end(),
        Volume{0},
        [](Volume volume, const auto& quote) { return quote.first + volume; });
    return TOB{bidPrice, bidVolume, askPrice, askVolume};
}

void OrderBook::SetBook(BidQuotesMap bidQuotes, AskQuotesMap askQuotes) {
    ASSERT(mQuotes.empty());
    mBidQuotes = bidQuotes;
    for (const auto& priceLevel : bidQuotes) {
        const auto price = priceLevel.first;
        for (const auto& quote : priceLevel.second) {
            auto inserted = mQuotes.emplace(
                quote.second,
                SingleQuoteDetails{Side::BUY, price, quote.first});
            ASSERT(inserted.second);
        }
    }
    mAskQuotes = askQuotes;
    for (const auto& priceLevel : askQuotes) {
        const auto price = priceLevel.first;
        for (const auto& quote : priceLevel.second) {
            auto inserted = mQuotes.emplace(
                quote.second,
                SingleQuoteDetails{Side::SELL, price, quote.first});
            ASSERT(inserted.second);
        }
    }
}

template <typename Compare>
Volume OrderBook::CrossBook(ClientId clientId, QuoteId quoteId, Price price, Volume volume,
                            std::map<Price, QuoteVec, Compare>& priceLevels) {
    const auto fillVolume = [&volume](auto& quote) {
        if (volume < quote.first) {
            quote.first -= volume;
            return true;
        }
        volume -= quote.first;
        return false;
    };
    const auto deleteQuote = std::bind(&OrderBook::FillQuote, this, Volume{0}, std::placeholders::_1);

    while (volume > 0 && !priceLevels.empty() &&
           !Compare()(price, priceLevels.begin()->first)) {
        auto& quotes = priceLevels.begin()->second;
        const auto prevVolume = volume;
        auto it = std::find_if(quotes.begin(), quotes.end(), fillVolume);

        std::for_each(quotes.begin(), it, deleteQuote);
        if (it != quotes.end()) {
            // Partially fill final quote
            if (volume != Volume{0}) FillQuote(volume, *it);
            quotes.erase(quotes.begin(), it);
            volume = Volume{0};
        }
        else
        {
            // Clear price level
            priceLevels.erase(priceLevels.begin());
        }
        mClientUpdater.SendFillPrivate(
            clientId, {quoteId, price, Volume{prevVolume - volume}});
    }
    return volume;
}
    
template <bool isBuy>
OrderBook::QuoteVec& OrderBook::GetPriceLevel(Price price) {
    if constexpr(isBuy) {
        return mBidQuotes[price];
    } else {
        return mAskQuotes[price];
    }
}

template <bool isBuy>
void OrderBook::DeletePriceLevel(Price price) {
    if constexpr(isBuy) {
        auto it = mBidQuotes.find(price);
        ASSERT(it != mBidQuotes.end());
        mBidQuotes.erase(it);
    } else {
        auto it = mAskQuotes.find(price);
        ASSERT(it != mAskQuotes.end());
        mAskQuotes.erase(it);
    }
}

}  // namespace FakeMarket
