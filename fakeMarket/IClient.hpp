#pragma once

#include "fakeMarket/events.hpp"

namespace FakeMarket {

class IClient {
 public:
  virtual ~IClient() = default;

  virtual ClientId GetClientId() const = 0;

  virtual void FAKResponse(const ResponseFAK&) = 0;

  virtual void QuoteUpdateResponse(const ResponseQuoteUpdate&) = 0;

  virtual void QuoteDeleteResponse(const ResponseQuoteDelete&) = 0;

  virtual void QuoteFillPrivate(const PrivateQuoteFill&) = 0;
};

}  // namespace FakeMarket
