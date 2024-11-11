#pragma once

enum class Side : std::int8_t
{
    BUY = 0,
    SELL = 1
}

BOOST_STRONG_TYPEDEF(std::uint8_t, ProductId);

BOOST_STRONG_TYPEDEF(int, Price);

BOOST_STRONG_TYPEDEF(unsigned, Volume);

BOOST_STRONG_TYPEDEF(std::uint8_t, QuoteId);
