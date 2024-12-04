#pragma once
#include <cstdint>
#include <variant>

#include "common/types.hpp"

namespace FakeMarket {

struct SubmitFAK {
    MsgNumber msgNumber;
    ClientId clientId;
    ProductId productId;
    Side side;
    Price price;
    Volume volume;
};

struct SubmitQuoteUpdate {
    MsgNumber msgNumber;
    ClientId clientId;
    ProductId productId;
    Side side;
    Price price;
    Volume volume;
    QuoteId quoteId;
};

struct SubmitQuoteDelete {
    MsgNumber msgNumber;
    ClientId clientId;
    ProductId productId;
    QuoteId quoteId;
};

using MarketOrder =
    std::variant<SubmitFAK, SubmitQuoteUpdate, SubmitQuoteDelete>;

enum class Result : std::int8_t {
    OK = 0,
    INVALID_PRODUCT = 1,
    QUOTE_DOESNT_EXIST = 2,
    PRICE_NA_TICK = 3,
    CANNOT_AMEND_QUOTE_SIDE
};

std::ostream& operator<<(std::ostream&, const Result&);

struct Response {
    MsgNumber msgNumber;
    Result result;
};

struct PrivateFill {
    QuoteId quoteId;
    Price price;
    Volume volume;
};

}  // namespace FakeMarket
