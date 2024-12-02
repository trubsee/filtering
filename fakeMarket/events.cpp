#include "fakeMarket/events.hpp"

namespace FakeMarket {

std::ostream& operator<<(std::ostream& os, const Result& res) {
    switch(res)
    {
        case Result::OK:
            os << "Result: OK";
            break;
        case Result::INVALID_PRODUCT:
            os << "Result: INVALID_PRODUCT";
            break;
        case Result::QUOTE_DOESNT_EXIST:
            os << "Result: QUOTE_DOESNT_EXIST";
            break;
        case Result::PRICE_NA_TICK:
            os << "Result: PRICE_NA_TICK";
            break;
        case Result::CANNOT_AMEND_QUOTE_SIDE:
            os << "Result: CANNOT_AMEND_QUOTE_SIDE";
            break;
    }
    return os;
}

}
