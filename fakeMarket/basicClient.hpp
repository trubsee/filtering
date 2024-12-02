#pragma once
#include <optional>
#include <queue>

#include "common/types.hpp"
#include "fakeMarket/IClient.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

class BasicClient : public IClient {
   public:
    BasicClient() : mClientId{nextClientId} { ++nextClientId; }

    ClientId GetClientId() const override { return mClientId; }

    void SendResponse(const Response& response) override {
        mResponses.push(response);
    }

    void FillPrivate(const PrivateFill& fill) override {
        mFills.push(fill);
    }

    std::optional<Response> GetResponse() {
        if (mResponses.empty()) return std::nullopt;
        const auto response = mResponses.front();
        mResponses.pop();
        return response;
    }
    
    std::optional<PrivateFill> GetFills() {
        if (mFills.empty()) return std::nullopt;
        const auto fill = mFills.front();
        mFills.pop();
        return fill;
    }

   private:
    static ClientId nextClientId;
    ClientId mClientId;
    std::queue<Response> mResponses;
    std::queue<PrivateFill> mFills;
};

}  // namespace FakeMarket
