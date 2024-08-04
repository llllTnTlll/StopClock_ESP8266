#include "screen.hpp"
#include <Ticker.h>

class Time{
private:
    struct tm
    {
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint16_t ms;
    };

    tm t;

public:
    Time(uint8_t hour, uint8_t min, uint8_t sec, uint16_t ms);
    Time();
    
    void AddTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    void MinusTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    String getCurrentTime();
    String getCurrentHour();
    String getCurrentMin();
    String getCurrentSec();
    String getCurrentMSec();

    // 运算符重载
    Time operator+(const Time& time) const{
        Time resultTime = Time(t.hour, t.min, t.sec, t.ms);
        resultTime.AddTime(time.t.hour, time.t.min, time.t.sec, time.t.ms);
        return resultTime;
    }
};

class StopWatch{
private:
    Screen* screen;        // 输出到的屏幕
    Time* time;            // 当前的时间结构体
    bool isRun;            // 是否处于计数状态
public:
    StopWatch(Screen*& myScreen);
    ~StopWatch();
    bool ReadRunStatus();
    void SetRunStatus(bool status);
    void ShowTime();
    void AddTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    void MinusTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    String GetCurrentTime();
};


