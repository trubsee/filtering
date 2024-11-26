#pragma once
#include <cstdint>
#include <ostream>

#include "boost/serialization/strong_typedef.hpp"

enum class Side : std::int8_t { BUY = 0, SELL = 1 };

BOOST_STRONG_TYPEDEF(std::uint16_t, ClientId);

BOOST_STRONG_TYPEDEF(std::uint16_t, ProductId);

BOOST_STRONG_TYPEDEF(double, Price);

BOOST_STRONG_TYPEDEF(std::uint32_t, Volume);

BOOST_STRONG_TYPEDEF(std::uint32_t, QuoteId);

struct TOB {
  Price bidPrice;
  Volume bidVolume;
  Price askPrice;
  Volume askVolume;

  bool operator==(const TOB& other) const {
    return bidPrice == other.bidPrice && bidVolume == other.bidVolume &&
           askPrice == other.askPrice && askVolume == other.askVolume;
  }
};

std::ostream& operator<<(std::ostream& os, const TOB& tob);
