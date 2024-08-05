#ifndef COMMON_H
#define COMMON_H

#include <ArxContainer.h>

class Font
{
protected:
    String errString;
    std::map<char, String> FontDic;

    virtual void InitFontDic() = 0;
    virtual void InitErrString() = 0;

public:
    inline String getOneCharCode(char word)
    {
        auto it = FontDic.find(word);
        if (it != FontDic.end())
        {
            return it->second;
        }
        else
        {
            return errString;
        }
    }
    void Initialize()
    {
        InitFontDic();
        InitErrString();
    }
};

// 参照以下格式创建自己的字体
class MyFont : public Font
{
protected:
    // 在这里定义你的字体
    void InitFontDic() override
    {
        FontDic['0'] = "1110111";
        FontDic['1'] = "0100100";
        FontDic['2'] = "1101011";
        FontDic['3'] = "1101101";
        FontDic['4'] = "0111100";
        FontDic['5'] = "1011101";
        FontDic['6'] = "1011111";
        FontDic['7'] = "1100100";
        FontDic['8'] = "1111111";
        FontDic['9'] = "1111101";

        FontDic[' '] = "0000000";
        FontDic['.'] = "0001000";
    }

    // 在这里定义字体缺失时的显示内容
    void InitErrString() override
    {
        errString = "0000000";
    }
};


class AnotherFont : public Font
{
protected:
    // 在这里定义你的字体
    void InitFontDic() override
    {
        FontDic['0'] = "11001001011";
        FontDic['1'] = "01000001000";
        FontDic['2'] = "11000110011";
        FontDic['3'] = "11000111001";
        FontDic['4'] = "01001111000";
        FontDic['5'] = "10001111001";
        FontDic['6'] = "10001111011";
        FontDic['7'] = "11000001000";
        FontDic['8'] = "11001111011";
        FontDic['9'] = "11001111001";

        FontDic[' '] = "00000000000";
    }

    // 在这里定义字体缺失时的显示内容
    void InitErrString() override
    {
        errString = "00000000000";
    }
};


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

#endif