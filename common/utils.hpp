#pragma once
#include <tuple>

#include "common/types.hpp"

namespace Common {

using HashId = std::uint64_t;

static HashId MakeHashId(ClientId cId, ProductId pId, QuoteId qId) {
    return (static_cast<HashId>(cId) | static_cast<HashId>(pId) << 16 |
            static_cast<HashId>(qId) << 32);
}

static std::tuple<ClientId, ProductId, QuoteId> ExtractHashId(HashId jId) {
    const auto cId = static_cast<ClientId>(jId & 0xFFFF);
    const auto pId = static_cast<ProductId>((jId >> 16) & 0xFFFF);
    const auto qId = static_cast<QuoteId>((jId >> 32) & 0xFFFFFFFF);
    return {cId, pId, qId};
}

}  // namespace Common
