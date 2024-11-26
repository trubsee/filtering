#pragma once
#include <chrono>
#include <unordered_set>

#include "common/assert.hpp"
#include "timer/timer.hpp"

namespace Timer {

template <typename Clock = std::chrono::steady_clock>
class SingleThreadedTimerManager {
   public:
    SingleThreadedTimerManager() {}

    unsigned CreateTimer(const Timer::ClockTime::duration& interval,
                         Timer::CallbackFunc callback) {
        Timer timer{Clock::now(), interval, callback};
        auto it = std::upper_bound(mTimers.begin(), mTimers.end(), timer,
                                   std::greater<Timer>());
        it = mTimers.insert(it, timer);
        return it->Id();
    }

    void DeleteTimer(unsigned id) {
        const auto it = std::find(mTimers.begin(), mTimers.end(), id);
        if (it != mTimers.end()) mTimers.erase(it);
    }

    void Update() {
        if (mTimers.empty()) return;
        while (mTimers[mTimers.size() - 1].CheckFire(Clock::now())) {
            auto timer = mTimers.back();
            mTimers.pop_back();
            const auto it = std::upper_bound(mTimers.begin(), mTimers.end(),
                                             timer, std::greater<Timer>());
            mTimers.insert(it, timer);
        }
    }

   private:
    std::vector<Timer> mTimers;
};

}  // namespace Timer
