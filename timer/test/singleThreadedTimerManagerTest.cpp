#include "gtest/gtest.h"

#include "timer/dummyClock.hpp"
#include "timer/singleThreadedTimerManager.hpp"

namespace Timer::Test {


TEST(SingleThreadedTimerManagerTest, Update)
{
    SingleThreadedTimerManager<DummyClock> timerManager; 
    
    timerManager.Update();

    int counter1 {0};
    auto id1 = timerManager.CreateTimer(
        std::chrono::seconds(3),
        [&counter1](){ ++counter1; }
    );
    
    int counter2 {0};
    auto id2 = timerManager.CreateTimer(
        std::chrono::seconds(2),
        [&counter2](){ ++counter2; }
    );
    
    int counter3 {0};
    auto id3 = timerManager.CreateTimer(
        std::chrono::seconds(5),
        [&counter3](){ ++counter3; }
    );

    DummyClock::tick(std::chrono::seconds(1));
    timerManager.Update();
    EXPECT_EQ(counter1, 0);
    EXPECT_EQ(counter2, 0);
    EXPECT_EQ(counter3, 0);
    
    DummyClock::tick(std::chrono::seconds(1));
    timerManager.Update();
    EXPECT_EQ(counter1, 0);
    EXPECT_EQ(counter2, 1);
    EXPECT_EQ(counter3, 0);
    
    DummyClock::tick(std::chrono::seconds(2));
    timerManager.Update();
    EXPECT_EQ(counter1, 1);
    EXPECT_EQ(counter2, 2);
    EXPECT_EQ(counter3, 0);

    timerManager.DeleteTimer(id2);
    
    DummyClock::tick(std::chrono::seconds(1));
    timerManager.Update();
    EXPECT_EQ(counter1, 1);
    EXPECT_EQ(counter2, 2);
    EXPECT_EQ(counter3, 1);
}

}
