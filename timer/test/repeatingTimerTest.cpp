#include "gtest/gtest.h"
#include "timer/timer.hpp"

namespace Timer::Test {

TEST(TimerTest, TestFire) {
    Timer::ClockTime start;
    int counter{0};
    Timer::ClockTime::duration interval{std::chrono::seconds(5)};
    Timer::CallbackFunc addCounter = [&counter]() { return ++counter; };

    Timer timer{start, interval, addCounter};
    EXPECT_EQ(timer.Next(), start + interval);

    auto now = start + std::chrono::seconds(3);
    auto hasFired = timer.CheckFire(now);
    EXPECT_EQ(hasFired, false);
    EXPECT_EQ(counter, 0);
    EXPECT_EQ(timer.Next(), start + interval);

    now += std::chrono::seconds(2);
    hasFired = timer.CheckFire(now);
    EXPECT_EQ(hasFired, true);
    EXPECT_EQ(counter, 1);
    EXPECT_EQ(timer.Next(), start + interval * 2);

    now += std::chrono::seconds(7);
    hasFired = timer.CheckFire(now);
    EXPECT_EQ(hasFired, true);
    EXPECT_EQ(counter, 2);
    EXPECT_EQ(timer.Next(), start + interval * 3);

    now += std::chrono::seconds(8);
    hasFired = timer.CheckFire(now);
    EXPECT_EQ(hasFired, true);
    EXPECT_EQ(counter, 3);
    EXPECT_EQ(timer.Next(), start + interval * 4);
}

TEST(TimerTest, TestComparison) {
    Timer::ClockTime start;
    Timer::ClockTime::duration interval{std::chrono::seconds(5)};
    Timer::CallbackFunc doNothing = []() {};
    Timer timer{start, interval, doNothing};
    EXPECT_TRUE(timer == 1);
    EXPECT_FALSE(timer == 2);

    Timer copyTimer{timer};
    EXPECT_TRUE(timer == copyTimer);

    Timer::ClockTime::duration slowInterval{std::chrono::seconds(7)};
    Timer slowTimer{start, slowInterval, doNothing};
    EXPECT_FALSE(timer == slowTimer);
    EXPECT_TRUE(timer < slowTimer);
    EXPECT_FALSE(slowTimer < timer);
}

}  // namespace Timer::Test
