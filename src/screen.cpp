#include <screen.hpp>

Screen::Screen(uint8_t gridNum, uint8_t segNum, uint8_t registerMaxRow, uint8_t registerMaxColumn)
    : screenGirdNum(gridNum), screenSegNum(segNum)
{
    driver = new PT63XX(screenGirdNum, screenSegNum, registerMaxRow, registerMaxColumn);
}

void Screen::SetOnoffLightness(bool onOff, uint8_t lightness)
{
    driver->PT63XX_SetScreen(onOff, lightness);
}

void Screen::ClearAll()
{
    driver->PT63XX_ClearAll();
}

void Screen::ClearBuffer(){
    driver->PT63XX_ClearBuffer();
}

void Screen::ClearScreen(){
    driver->PT63XX_ClearRegister();
}

void Screen::TestScreen()
{
    driver->PT63XX_Test(screenGirdNum, screenSegNum);
}

void Screen::WriteString(String content, std::vector<bool> symbolMask)
{
    // ClearScreen();
    // 显示所有的符号
    DisplaySymbol(symbolMask);

    // 处理显示所有的8字位
    uint8_t digitPos = 0;
    content = content.substring(0, 7);

    for (uint8_t i = 0; i < content.length(); ++i)
    {
        char word = static_cast<char>(content[i]);

        Coordinate coord = ScreenDigitBeginIndex[digitPos];
        driver->PT63XX_WriteBufferBits(coord.gridIndex, coord.segIndex, ToBoolVec(font->getOneCharCode(word)));

        digitPos += 1;
    }

    driver->PT63XX_ShowFrame();
}

void Screen::WriteNum(double num, uint8_t width, uint8_t prec, std::vector<bool> symbolMask)
{
    char buffer[width + 1];
    dtostrf(num, width, prec, buffer);
    for (size_t i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ' ') {
            buffer[i] = '0';
        }
    }
    String content = String(buffer);
    
    WriteString(content, symbolMask);
}

void Screen::WriteNum(int num, uint8_t width, uint8_t prec, std::vector<bool> symbolMask){
    char buffer[width + 1];
    dtostrf(num, width, prec, buffer);
    for (size_t i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ' ') {
            buffer[i] = '0';
        }
    }
    String content = String(buffer);
    
    WriteString(content, symbolMask);
}

void Screen::DisplaySymbol(const std::vector<bool> symbolMask)
{
    if (symbolMask.size() != ScreenSymbolsIndex.size()) {
        Serial.println("Error: symbolMask and ScreenSymbolsIndex size mismatch.");
        return;
    }

    for (size_t index = 0; index < ScreenSymbolsIndex.size(); ++index)
    {
        if (symbolMask[index])
            driver->PT63XX_WriteBufferOneBit(ScreenSymbolsIndex[index].gridIndex, ScreenSymbolsIndex[index].segIndex, 1);
        else
            driver->PT63XX_WriteBufferOneBit(ScreenSymbolsIndex[index].gridIndex, ScreenSymbolsIndex[index].segIndex, 0);
    }
}

void Screen::ScrollPirnt(String content, uint16_t interval)
{
    WriteString(content);
    delay(interval);
    while (content.length() > 7)
    {
        content = content.substring(1);
        WriteString(content);
        delay(interval);
    }
    delay(interval);
}

Screen::~Screen()
{
    driver->~PT63XX();
}

void Screen::Initialize(Font *f)
{
    font = f;
    font->Initialize();
    SetDigitBeginIndex();
    SetSymbolsIndex();
}

void Screen::Print(){
    driver->PT63XX_ShowFrame();
}

void Screen::WriteBit(uint8_t grid, uint8_t seg, bool bit){
    driver->PT63XX_WriteBufferOneBit(grid, seg, bit);
}