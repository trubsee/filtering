#pragma once
#include "boost/serialization/strong_typedef.hpp"

enum class Side : std::int8_t
{
    BUY = 0,
    SELL = 1
};

BOOST_STRONG_TYPEDEF(std::uint16_t, ClientId);

BOOST_STRONG_TYPEDEF(std::uint16_t, ProductId);

BOOST_STRONG_TYPEDEF(double, Price);

BOOST_STRONG_TYPEDEF(std::uint32_t, Volume);

BOOST_STRONG_TYPEDEF(std::uint32_t, QuoteId);

struct TOB
{
    Price bidPrice;
    Volume bidVolume;
    Price askPrice;
    Volume askVolume;
};
