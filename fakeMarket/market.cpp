#include "fakeMarket/market.hpp"

namespace FakeMarket {

Market::Market(unsigned lifetime, double tickSize)
:
    mLifetime{lifetime},
    mLastClientId{0}
{
    mOrderBooks.emplace(1, OrderBook{0.1});
}

ClientId Market::AddClient()
{
    ++mLastClientId;
    return mLastClientId;
}

void Market::Run()
{
    std::unique_lock<std::mutex> lock(mOrderMutex);
    for (unsigned i; i < mLifetime; ++i)
    {
        mOrderCV.wait(lock, [this](){ return !mOrders.empty(); });
        const MarketOrder order = mOrders.front();
        mOrders.pop();
        lock.unlock();
    }
}

void Market::AddToOrders(const MarketOrder& order)
{
    std::lock_guard<std::mutex> lock(mOrderMutex);
    mOrders.push(order);
    mOrderCV.notify_one();
}

void Market::ProcessOrder(const MarketOrder& order)
{
    auto pId = std::visit(
        [](auto&& obj) { return obj.productId; }, 
        order
    );
    auto it = mOrderBooks.find(pId);
    if (it == mOrderBooks.end())
        return;

    if (const auto qd = std::get_if<SubmitQuoteDelete>(&order))
        it->second.QuoteDelete(*qd);

    if (const auto fak = std::get_if<SubmitFAK>(&order))
        it->second.FAK(*fak);

    if (const auto qu = std::get_if<SubmitQuoteUpdate>(&order))
        it->second.QuoteUpdate(*qu);
}

}
