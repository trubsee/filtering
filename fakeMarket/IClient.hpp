#pragma once

#include "fakeMarket/events.hpp"

namespace FakeMarket {

class IClient {
   public:
    virtual ~IClient() = default;

    virtual ClientId GetClientId() const = 0;

    virtual void SendResponse(const Response&) = 0;

    virtual void FillPrivate(const PrivateFill&) = 0;
};

}  // namespace FakeMarket
