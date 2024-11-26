#pragma once

#include "common/types.hpp"
#include "fakeMarket/IClient.hpp"
#include "fakeMarket/events.hpp"

namespace FakeMarket {

class Client : public IClient {
   public:
    Client() : mClientId{nextClientId} { ++nextClientId; }

    ClientId GetClientId() const override { return mClientId; }

    void FAKResponse(const ResponseFAK&) override {}

    void QuoteUpdateResponse(const ResponseQuoteUpdate&) override {}

    void QuoteDeleteResponse(const ResponseQuoteDelete&) override {}

    void QuoteFillPrivate(const PrivateQuoteFill&) override {}

   private:
    static ClientId nextClientId;
    ClientId mClientId;
};

}  // namespace FakeMarket
