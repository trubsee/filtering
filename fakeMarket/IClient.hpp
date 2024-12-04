#pragma once

#include "fakeMarket/events.hpp"

namespace FakeMarket {

class IClient {
   public:
    virtual ~IClient() = default;

    virtual void RegisterFAK(const SubmitFAK& fak) = 0;

    virtual void RegisterQuoteUpdate(const SubmitQuoteUpdate& quoteUpdate) = 0;

    virtual void RegisterQuoteDelete(const SubmitQuoteDelete& quoteDelete) = 0;

    virtual ClientId GetClientId() const = 0;

    virtual void SendResponse(const Response&) = 0;

    virtual void FillPrivate(const PrivateFill&) = 0;
};

}  // namespace FakeMarket
