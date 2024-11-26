#include "common/types.hpp"


std::ostream& operator<<(std::ostream& os, const TOB& tob)
{
    os << "Bid: @" << tob.bidPrice << "/" << tob.bidVolume
        << " | Ask: @" << tob.askPrice << "/" << tob.askVolume;
    return os;
}
