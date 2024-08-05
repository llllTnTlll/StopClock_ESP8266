#include "funcs.hpp"

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
    int lastSec = time->getCurrentSec().toInt();
    time->AddTime(hours, minutes, seconds, milliseconds);
    int nowSec = time->getCurrentSec().toInt();
    if (lastSec != nowSec)
    {
        shouldBink = true;
    }
}

void StopWatch::MinusTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds)
{
    int lastSec = time->getCurrentSec().toInt();
    time->MinusTime(hours, minutes, seconds, milliseconds);
    int nowSec = time->getCurrentSec().toInt();
    if (lastSec != nowSec)
    {
        shouldBink = true;
    }
}

bool StopWatch::ReadRunStatus()
{
    return isRun;
}

void StopWatch::SetRunStatus(bool status)
{

    isRun = status;
}

String StopWatch::GetCurrentTime()
{
    return time->getCurrentMin() + time->getCurrentSec() + time->getCurrentMSec();
}

bool StopWatch::GetBink()
{
    if (shouldBink)
    {
        shouldBink = false;
        return true;
    }
    else
    {
        return false;
    }
}