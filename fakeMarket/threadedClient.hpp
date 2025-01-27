#pragma once
#include <functional>
#include <optional>
#include <queue>

#include "concurrentqueue.h"

#include "common/types.hpp"
#include "fakeMarket/IClient.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

class ThreadedClient : public IClient {
   public:
    ThreadedClient(std::function<void(const MarketOrder&)> addToOrders)
        : mClientId{nextClientId}, mAddToOrders{addToOrders} {
        ++nextClientId;
    }

    ClientId GetClientId() const override { return mClientId; }

    void RegisterFAK(const SubmitFAK& fak) override { mAddToOrders(fak); }

    void RegisterQuoteUpdate(const SubmitQuoteUpdate& quoteUpdate) override {
        mAddToOrders(quoteUpdate);
    }

    void RegisterQuoteDelete(const SubmitQuoteDelete& quoteDelete) override {
        mAddToOrders(quoteDelete);
    }

    void SendResponse(const Response& response) override {
        mResponses.enqueue(response);
    }

    void FillPrivate(const PrivateFill& fill) override { mFills.enqueue(fill); }

    std::optional<Response> GetResponse() {
        Response response;
        if (mResponses.try_dequeue(response))
        {
            return response;
        }
        return std::nullopt;
    }

    std::optional<PrivateFill> GetFill() {
        PrivateFill fill;
        if (mFills.try_dequeue(fill))
        {
            return fill;
        }
        return std::nullopt;
    }

   private:
    static ClientId nextClientId;
    ClientId mClientId;

    std::function<void(const MarketOrder&)> mAddToOrders;
    moodycamel::ConcurrentQueue<Response> mResponses;
    moodycamel::ConcurrentQueue<PrivateFill> mFills;
};

}  // namespace FakeMarket

