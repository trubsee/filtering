#include "common/mmapReader.hpp"
#include "common/mmapWriter.hpp"
#include "gtest/gtest.h"

namespace Common::Test {

struct DummyMessage
{
    int mInt;
    bool mBool;

    bool operator==(const DummyMessage& other) const { return mInt == other.mInt && mBool == other.mBool; }
};


TEST(MMapTest, SimpleReadWrite)
{
    const std::string filename {"test_file"};
    MMapWriter<DummyMessage> writer(filename);
    MMapReader<DummyMessage> reader(filename);

    DummyMessage first {10, true};
    writer.Write(first);
    EXPECT_EQ(first, reader.Read());
    EXPECT_EQ(first, reader.Read());
    
    DummyMessage second {8, false};
    writer.Write(second);
    EXPECT_EQ(second, reader.Read());
}

}
