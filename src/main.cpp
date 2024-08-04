#include <Arduino.h>
#include <Ticker.h>
#include "screen.hpp"
#include "timer.hpp"

Screen *myScreen;
StopWatch *myStopWatch;
Ticker ticker;

std::vector<bool> symbolMask = {1, 1};
uint8_t counter = 0;

enum RunMode
{
  COUNTING_MODE,
  TIME_ADJUST_MODE,
};

RunMode CURRENT_RUN_MODE;

// 按键
bool isBtnAPressed = false;
bool isBtnBPressed = false;
bool isBtnCPressed = false;

// 强力delay
void StrongDelay(unsigned long ms)
{
  unsigned long currentMillis = millis();
  while (millis() - currentMillis < ms)
    ;
}

// 中断响应函数
void IRAM_ATTR Btn_A_Pressed()
{
  StrongDelay(10);
  if (digitalRead(D1) == LOW)
  {
    isBtnAPressed = true;
  }
}

void IRAM_ATTR Btn_B_Pressed()
{
  StrongDelay(10);
  if (digitalRead(D2) == LOW)
  {
    isBtnBPressed = true;
  }
}

void IRAM_ATTR Btn_C_Pressed()
{
  StrongDelay(10);
  if (digitalRead(D3) == LOW)
  {
    isBtnCPressed = true;
  }
}

void OnTicker()
{
  if (myStopWatch->ReadRunStatus())
  {
    if (counter >= 100)
    {
      for (std::size_t i = 0; i < symbolMask.size(); ++i)
      {
        symbolMask[i] = !symbolMask[i];
      }
      myScreen->DisplaySymbol(symbolMask);
      counter = 0;
    }
    myStopWatch->MinusTime(0, 0, 0, 10);
    if(myStopWatch->GetCurrentTime() == "000000"){
      myStopWatch->SetRunStatus(0);
    }
    counter++;
  }
  else
  {
    return;
  }
}

void setup()
{
  // 启动调试串口
  Serial.begin(9600);

  // 初始化屏幕和时钟
  myScreen = new SamsungScreen(6, 16);
  myStopWatch = new StopWatch(myScreen);

  // 设置当前运行模式为时间调整模式
  CURRENT_RUN_MODE = TIME_ADJUST_MODE;

  // 绑定按键中断
  pinMode(D1, INPUT_PULLUP);
  attachInterrupt(D1, Btn_A_Pressed, FALLING); // D1为加1min
  pinMode(D2, INPUT_PULLUP);
  attachInterrupt(D2, Btn_B_Pressed, FALLING); // D1为加10S
  pinMode(D3, INPUT_PULLUP);
  attachInterrupt(D3, Btn_C_Pressed, FALLING); // C 开始停止

  // 启动定时器
  ticker.attach_ms(10, OnTicker);
}

void loop()
{
  // 按键响应
  switch (CURRENT_RUN_MODE)
  {
  case TIME_ADJUST_MODE:
    if (isBtnAPressed)
    {
      myStopWatch->AddTime(0, 1, 0, 0);
    }
    if (isBtnBPressed)
    {
      myStopWatch->AddTime(0, 0, 10, 0);
    }
    if (isBtnCPressed)
    {
      myStopWatch->SetRunStatus(!myStopWatch->ReadRunStatus());
    }
    break;

  default:
    break;
  }
  // 清除所有标志位
  isBtnAPressed = false;
  isBtnBPressed = false;
  isBtnCPressed = false;

  myStopWatch->ShowTime();
}
