#include "timer.hpp"

StopWatch::StopWatch(Screen *&myScreen)
    : screen(myScreen), isRun(false)
{
    time = new Time();
}

StopWatch::~StopWatch()
{
    screen->~Screen();
}

void StopWatch::ShowTime()
{
    String currentTime = time->getCurrentTime();
    screen->WriteString(currentTime);
    screen->Print();
}

void StopWatch::AddTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds)
{
    time->AddTime(hours, minutes, seconds, milliseconds);
}

void StopWatch::MinusTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds)
{
    time->MinusTime(hours, minutes, seconds, milliseconds);
}

bool StopWatch::ReadRunStatus(){
    return isRun;
}

void StopWatch::SetRunStatus(bool status)
{
    isRun = status;
}

String StopWatch::GetCurrentTime(){
    return time->getCurrentMin() + time->getCurrentSec() + time->getCurrentMSec();
}

Time::Time(uint8_t h, uint8_t m, uint8_t s, uint16_t ms)
{
    t.hour = h;
    t.min = m;
    t.sec = s;
    t.ms = ms;
}

Time::Time()
{
    t.hour = 0;
    t.min = 0;
    t.sec = 0;
    t.ms = 0;
}

void Time::AddTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds)
{
    t.ms += milliseconds;
    if (t.ms >= 1000)
    {
        t.sec += t.ms / 1000;
        t.ms %= 1000;
    }

    t.sec += seconds;
    if (t.sec >= 60)
    {
        t.min += t.sec / 60;
        t.sec %= 60;
    }

    t.min += minutes;
    if (t.min >= 60)
    {
        t.hour += t.min / 60;
        t.min %= 60;
    }

    t.hour += hours;

    // 处理超过24小时
}

void Time::MinusTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds)
{
    // 减去毫秒
    if (t.ms < milliseconds)
    {
        t.ms += 1000; // 借位
        if (t.sec > 0) {
            t.sec--; // 处理秒数减少
        } else {
            t.sec = 59; // 秒数下溢
            if (t.min > 0) {
                t.min--; // 处理分钟减少
            } else {
                t.min = 59; // 分钟下溢
                if (t.hour > 0) {
                    t.hour--; // 处理小时减少
                } else {
                    t.hour = 23; // 小时下溢
                }
            }
        }
    }
    t.ms -= milliseconds;

    // 减去秒数
    if (t.sec < seconds)
    {
        t.sec += 60; // 借位
        if (t.min > 0) {
            t.min--; // 处理分钟减少
        } else {
            t.min = 59; // 分钟下溢
            if (t.hour > 0) {
                t.hour--; // 处理小时减少
            } else {
                t.hour = 23; // 小时下溢
            }
        }
    }
    t.sec -= seconds;

    // 减去分钟
    if (t.min < minutes)
    {
        t.min += 60; // 借位
        if (t.hour > 0) {
            t.hour--; // 处理小时减少
        } else {
            t.hour = 23; // 小时下溢
        }
    }
    t.min -= minutes;

    // 减去小时
    t.hour -= hours;

    // 确保时间值在合理范围内
    if (t.hour < 0) {
        t.hour = 0;
        t.min = 0;
        t.sec = 0;
        t.ms = 0;
    }
}

String Time::getCurrentTime()
{
    String timeStr = " ";           // TODO 这个地方可以通过添加空位，来保证显示对齐
    // timeStr += getCurrentHour();
    // timeStr += ":";
    timeStr += getCurrentMin();
    // timeStr += ":";
    timeStr += getCurrentSec();
    // timeStr += ".";
    timeStr += getCurrentMSec();

    return timeStr;
}

String Time::getCurrentHour()
{
    String hour = String(t.hour);
    if (t.hour < 10)
        hour = "0" + hour;
    return hour.substring(0, 2);
}

String Time::getCurrentMin()
{
    String min = String(t.min);
    if (t.min < 10)
        min = "0" + min;
    return min.substring(0, 2);
}

String Time::getCurrentSec()
{
    String sec = String(t.sec);
    if (t.sec < 10)
        sec = "0" + sec;
    return sec.substring(0, 2);
}

String Time::getCurrentMSec()
{
    String ms = String(t.ms);
    if (t.ms < 10)
        ms = "00" + ms;
    else if (t.ms < 100)
        ms = "0" + ms;

    return ms.substring(0, 2); // 这里屏幕只能显示两位所以只保留了两位
}