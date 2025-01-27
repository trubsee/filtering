#include "fakeMarket/market.hpp"

namespace FakeMarket {

Market::Market(double tickSize) : mClientUpdater{mEventDispatcher} {
    mOrderBooks.emplace(1, OrderBook{mClientUpdater, 0.1});
}

BasicClient Market::AddClient() {
    BasicClient client{
        [this](const MarketOrder& order) { AddToOrders(order); }};
    mClientUpdater.ConnectClient(client);
    return client;
}

void Market::Tick() {
    MarketOrder order;
    while (mOrders.try_dequeue(order)) {
        ProcessOrder(order);
    }
    std::this_thread::yield();
}

void Market::AddToOrders(const MarketOrder& order) { mOrders.enqueue(order); }

void Market::ProcessOrder(const MarketOrder& order) {
    const auto pId =
        std::visit([](auto&& obj) { return obj.productId; }, order);
    auto it = mOrderBooks.find(pId);
    if (it == mOrderBooks.end()) {
        const auto msgNumber =
            std::visit([](auto&& obj) { return obj.msgNumber; }, order);
        const auto clientId =
            std::visit([](auto&& obj) { return obj.clientId; }, order);
        mClientUpdater.SendResponse(clientId,
                                    {msgNumber, Result::INVALID_PRODUCT});
        return;
    }

    if (const auto qd = std::get_if<SubmitQuoteDelete>(&order))
        it->second.QuoteDelete(*qd);

    if (const auto fak = std::get_if<SubmitFAK>(&order)) it->second.FAK(*fak);

    if (const auto qu = std::get_if<SubmitQuoteUpdate>(&order))
        it->second.QuoteUpdate(*qu);
}

}  // namespace FakeMarket
