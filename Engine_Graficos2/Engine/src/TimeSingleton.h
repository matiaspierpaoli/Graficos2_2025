#pragma once
#include "Exports.h"
#include "Time/Time.h"

class GraficosEngine_API TimeSingleton
{
private:
    Time* GetTimeInternal() { return time; }
    void SetTimeInternal(Time* newTime) { time = newTime; }
    TimeSingleton() {}
    Time* time;

public:
    TimeSingleton(const TimeSingleton&) = delete;
    static TimeSingleton& Get()
    {
        static TimeSingleton instance;
        return instance;
    }
    static Time* GetTime() { return Get().GetTimeInternal(); }
    static void SetTime(Time* newTime) { Get().SetTimeInternal(newTime); }
};