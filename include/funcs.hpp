#include "screen.hpp"
#include <Ticker.h>

class StopWatch{
private:
    Screen* screen;        // 输出到的屏幕
    Time* time;            // 当前的时间结构体
    bool isRun;            // 是否处于计数状态
    bool shouldBink;       // 冒号闪烁标志位
public:
    StopWatch(Screen*& myScreen);
    ~StopWatch();
    bool ReadRunStatus();
    void SetRunStatus(bool status);

    void ShowTime();
    void AddTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);
    void MinusTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t milliseconds);

    String GetCurrentTime();
    bool GetBink();
};


