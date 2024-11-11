#pragma once
#include <chrono>
#include <functional>

namespace Common::Timer {

class Timer
{
public:
    using ClockTime = std::chrono::steady_clock::time_point;
    using CallbackFunc = std::function<void()>;

    Timer(ClockTime now, ClockTime::duration interval, CallbackFunc callback)
    :
        mId{nextId},
        mInterval{interval},
        mCallback{callback},
        mNext{now + mInterval} 
    {
        ++nextId;
    }
    
    const ClockTime& Next() const { return mNext; }
   
    unsigned Id() const { return mId; }

    bool CheckFire(const ClockTime& now)
    { 
        if (now >= mNext)
        {
            mCallback();
            mNext += mInterval;
            return true;
        }
        return false;
    }
        
    bool operator>(const Timer& other) const { return Next() > other.Next(); }
    bool operator<(const Timer& other) const { return Next() < other.Next(); }
    bool operator==(const Timer& other) const { return Id() == other.Id(); }
    bool operator==(unsigned id) const { return Id() == id; }

    static unsigned nextId;
    
private:
    unsigned mId;
    ClockTime::duration mInterval;
    CallbackFunc mCallback;
    ClockTime mNext;
};

}
