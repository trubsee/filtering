#pragma once

#include "common/types.hpp"
#include "fakeMarket/IClient.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

class Client : public IClient {
   public:
    Client() : mClientId{nextClientId} { ++nextClientId; }

    ClientId GetClientId() const override { return mClientId; }

    void SendResponse(const Response&) override {}

    void FillPrivate(const PrivateFill&) override {}

   private:
    static ClientId nextClientId;
    ClientId mClientId;
};

}  // namespace FakeMarket
