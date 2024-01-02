#include "Timer.h"

#include <iostream>

void Timer::Reset()
{
    m_StartTime = std::chrono::system_clock::now();
}

std::chrono::microseconds Timer::GetTimeSpan()
{
    const auto end_time = std::chrono::system_clock::now();
    const auto start =
        std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime);
    const auto end =
        std::chrono::time_point_cast<std::chrono::microseconds>(end_time);

    return end - start;
}

long long Timer::GetTimeSpanInLongLong()
{
    const auto end_time = std::chrono::system_clock::now();
    const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
    const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();

    const auto duration = end - start;
    //double ms = duration * 0.001;
    return duration;
}

LogTimer::~LogTimer()
{
    const auto end_time = std::chrono::system_clock::now();
    const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
    const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();
    
    const auto duration = end - start;
    //double ms = duration * 0.001;
    std::cout << f << "___" << duration << std::endl;
}
