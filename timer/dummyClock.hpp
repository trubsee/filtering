#pragma once

#include "timer/timer.hpp"

namespace Timer {

class DummyClock {
   public:
    static void time(Timer::ClockTime now) { mTime = now; }

    static void tick(Timer::ClockTime::duration elapsed) { mTime += elapsed; }

    static const Timer::ClockTime& now() { return mTime; }

   private:
    static Timer::ClockTime mTime;
};

}  // namespace Timer
