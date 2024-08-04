#ifndef PT63XX_HPP
#define PT63XX_HPP

#include <SPI.h>
#include <ArxContainer.h>

// 需要根据使用的片选引脚重新定义
#define SPI_CS 15

// command1
/*
 * 显示模式设定
 * 0 0 - - b3 b2 b1 b0
 *
 * b3~b0: 0100 8区域16分隔符
 */
typedef enum
{
    SCAN_MODE_4D24S = 0b00000000,
    SCAN_MODE_5D23S = 0b00000001,
    SCAN_MODE_6D22S = 0b00000010,
    SCAN_MODE_7D21S = 0b00000011,
    SCAN_MODE_8D20S = 0b00000100,
    SCAN_MODE_9D19S = 0b00000101,
    SCAN_MODE_10D18S = 0b00000110,
    SCAN_MODE_11D17S = 0b00000111,
    SCAN_MODE_12D16S = 0b00001000,
} CMD1_SCAN_MODE;

// command2
/*
 * 数据设置命令
 * 0 1 - - b3 b2 b1 b0
 *
 * b3: 0 for 普通模式
 *     1 for 测试模式
 *
 * b2: 0 for 地址自增
 *     1 for 指定地址
 *
 * b1&b0: 00 向显示器寄存器写数据
 *        01 向驱动芯片LED写数据
 *        10 读取矩阵键盘数据
 *        11 无效
 *
 */
typedef enum
{
    AUTO_ADR_WRITE_DISPLAY = 0b01000000,
    FIXED_ADR_WRITE_DISPLAY = 0b01000100,
} CMD2_DTA_SET;

class PT63XX
{
private:
    bool **Buffer;
    uint8_t registerMaxRow;
    uint8_t registerMaxColumn;
    uint8_t registerBufferLen;

    uint8_t screenMaxGrid;
    uint8_t screenMaxSeg;

    CMD1_SCAN_MODE CURRENT_SCAN_MODE;
    uint8_t CURRENT_LIGHTNESS;

    CMD1_SCAN_MODE getScanModeCMD(uint8_t gridNum);
    uint8_t getSetMemToCMD(uint8_t memIndex);

    void PT63XX_SendCMD(uint8_t cmd);
    void PT63XX_SendDTA_AutoAdr(uint8_t *sendBuf);
    uint8_t *PT63XX_GetSendBuf();

    inline uint8_t reverseByte(uint8_t byte)
    {
        uint8_t reversed = 0;
        for (int i = 0; i < 8; i++)
        {
            reversed <<= 1;
            reversed |= (byte & 1);
            byte >>= 1;
        }
        return reversed;
    }

public:
    PT63XX(uint8_t screenGridNum, uint8_t screenSegNum, uint8_t registerMaxRow = 12, uint8_t registerMaxColumn = 24);
    ~PT63XX();

    void PT63XX_WriteBufferOneBit(uint8_t grid, uint8_t seg, bool bit);
    void PT63XX_WriteBufferBits(uint8_t grid, uint8_t seg, std::vector<bool>bits);

    void PT63XX_ShowFrame();
    void PT63XX_ClearAll();
    void PT63XX_ClearBuffer();
    void PT63XX_ClearRegister();

    void PT63XX_SetScreen(bool onOff, uint8_t lightness);
    void PT63XX_SetScanMode(CMD1_SCAN_MODE scanMode);
    void PT63XX_Test(uint8_t screenGridNum, uint8_t screenSegNum);
};

#endif