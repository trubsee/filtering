#include "common/types.hpp"
#include "common/utils.hpp"
#include "gtest/gtest.h"

namespace Common::Test {

TEST(UtilsTest, HashIdConversion) {
    ClientId cId{3};
    ProductId pId{100};
    QuoteId qId{12000};
    const auto hId = MakeHashId(cId, pId, qId);
    const auto [cId_, pId_, qId_] = ExtractHashId(hId);
    EXPECT_EQ(cId, cId_);
    EXPECT_EQ(pId, pId_);
    EXPECT_EQ(qId, qId_);
}

}  // namespace Common::Test
