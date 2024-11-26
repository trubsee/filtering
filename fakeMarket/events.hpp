#pragma once
#include "common/types.hpp"

namespace FakeMarket {

struct SubmitFAK {
  ClientId clientId;
  ProductId productId;
  Side side;
  Price price;
  Volume volume;
};

struct SubmitQuoteUpdate {
  ClientId clientId;
  ProductId productId;
  Side side;
  Price price;
  Volume volume;
  QuoteId quoteId;
};

struct SubmitQuoteDelete {
  ClientId clientId;
  ProductId productId;
  QuoteId quoteId;
};

struct ResponseFAK {};

struct ResponseQuoteUpdate {};

struct ResponseQuoteDelete {};

struct PrivateQuoteFill {};

}  // namespace FakeMarket
