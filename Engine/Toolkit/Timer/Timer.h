#pragma once
#include <chrono>

// 栈计时器
class Timer
{
public:
    Timer(const char* fun) : m_StartTime(std::chrono::system_clock::now()), f(fun) {}

    void Reset();
    
    std::chrono::microseconds GetTimeSpan();

    long long GetTimeSpanInLongLong();

protected:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    const char* f;
};

#include <chrono>
#include <thread>

class PreciseSleep {
public:
    static void sleep_for_milliseconds(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

private:
    void exampal()
    {
        PreciseSleep::sleep_for_milliseconds(500);
    }
};

class LogTimer : public Timer
{
    ~LogTimer();
};
#define T() LogTimer LogTimer##__LINE__(__func__)