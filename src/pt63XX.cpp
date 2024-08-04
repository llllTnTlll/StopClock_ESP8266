#include <pt63XX.hpp>

PT63XX::PT63XX(uint8_t screenGridNum, uint8_t screenSegNum, uint8_t maxRow, uint8_t maxColumn)
    : registerMaxRow(maxRow),
      registerMaxColumn(maxColumn),
      registerBufferLen(registerMaxRow * (registerMaxColumn / 8)),
      screenMaxGrid(screenGridNum),
      screenMaxSeg(screenSegNum),
      CURRENT_SCAN_MODE((CMD1_SCAN_MODE)getScanModeCMD(screenMaxGrid)),
      CURRENT_LIGHTNESS(7)
{
    // 初始化Buffer
    Buffer = new bool *[registerMaxRow];
    for (int i = 0; i < registerMaxRow; ++i)
    {
        Buffer[i] = new bool[registerMaxColumn]{0};
    }

    // 初始化spi
    SPI.begin();
    SPI.beginTransaction(SPISettings(8000000, LSBFIRST, SPI_MODE3));
    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);

    // 以地址自增模式初始化芯片
    delay(200);
    PT63XX_ClearAll();
    PT63XX_SetScanMode(CURRENT_SCAN_MODE);
    PT63XX_SetScreen(1, CURRENT_LIGHTNESS);
}

PT63XX::~PT63XX()
{
    // 析构时销毁Buffer
    for (int i = 0; i < registerMaxRow; ++i)
    {
        delete[] Buffer[i];
    };
    delete[] Buffer;
}

void PT63XX::PT63XX_SendCMD(uint8_t cmd)
{
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(cmd);
    digitalWrite(SPI_CS, HIGH);
    delay(1);
}

void PT63XX::PT63XX_SendDTA_AutoAdr(uint8_t *sendBuf)
{
    PT63XX_SendCMD(AUTO_ADR_WRITE_DISPLAY);

    digitalWrite(SPI_CS, LOW);
    SPI.transfer(getSetMemToCMD(0));

    for (int i = 0; i < registerBufferLen; i++)
    {
        SPI.transfer(sendBuf[i]);
    }

    digitalWrite(SPI_CS, HIGH);
    delay(10);
}

void PT63XX::PT63XX_ClearAll()
{
    PT63XX_ClearBuffer();

    PT63XX::PT63XX_ClearRegister();
}

void PT63XX::PT63XX_ClearBuffer()
{
    for (int i = 1; i <= registerMaxRow; ++i)
    {
        for (int j = 1; j <= registerMaxColumn; ++j)
        {
            PT63XX_WriteBufferOneBit(i, j, 0);
        }
    }
}

void PT63XX::PT63XX_ClearRegister()
{
    uint8_t *buf = PT63XX_GetSendBuf();
    PT63XX_SendDTA_AutoAdr(buf);
    delete[] buf;
}

// 危险，注意内存溢出
uint8_t *PT63XX::PT63XX_GetSendBuf()
{
    uint8_t *flattenBuf = new uint8_t[registerBufferLen];
    int k = 0;
    for (int i = 0; i < registerMaxRow; ++i)
    {
        for (int j = 0; j < registerMaxColumn; j += 8)
        {
            uint8_t byte = 0;
            for (int bit = 0; bit < 8; ++bit)
            {
                byte |= (Buffer[i][j + bit] << (7 - bit));
            }
            flattenBuf[k++] = reverseByte(byte);
        }
    }
    return flattenBuf;
}

void PT63XX::PT63XX_ShowFrame()
{
    PT63XX_SendCMD(AUTO_ADR_WRITE_DISPLAY);
    PT63XX_SendCMD(getSetMemToCMD(0));
    uint8_t *buf = PT63XX_GetSendBuf();
    PT63XX_SendDTA_AutoAdr(buf);
    delete[] buf;
    PT63XX_SetScanMode(CURRENT_SCAN_MODE);
    PT63XX_SetScreen(1, CURRENT_LIGHTNESS);
}

void PT63XX::PT63XX_WriteBufferOneBit(uint8_t grid, uint8_t seg, bool bit)
{
    Buffer[grid - 1][seg - 1] = bit;
}

void PT63XX::PT63XX_WriteBufferBits(uint8_t grid, uint8_t seg, std::vector<bool> bits)
{
    for (size_t i = 0; i < bits.size(); i++)
    {
        Buffer[grid - 1][seg - 1 + i] = static_cast<bool>(bits[i]);
    }
}

void PT63XX::PT63XX_SetScreen(bool onOff, uint8_t lightness)
{
    if (lightness <= 7)
    {
        uint8_t cmd4_temp = 0b10000000;
        if (onOff)
        {
            cmd4_temp |= 0b00001000;
        }
        cmd4_temp |= lightness;
        PT63XX_SendCMD(cmd4_temp);
    }
}

void PT63XX::PT63XX_SetScanMode(CMD1_SCAN_MODE scanMode)
{
    PT63XX_SendCMD(scanMode);
}

void PT63XX::PT63XX_Test(uint8_t screenGridNum, uint8_t screenSegNum)
{
    // 每一段逐次亮起
    for (int i = 1; i <= screenSegNum; ++i)
    {
        for (int j = 1; j <= screenGridNum; ++j)
        {
            PT63XX_WriteBufferOneBit(j, i, 1);
        }
        PT63XX_ShowFrame();
        delay(1000);
        PT63XX_ClearAll();
    }

    // 全部亮起
    for (int i = 1; i <= screenSegNum; ++i)
    {
        for (int j = 1; j <= screenGridNum; ++j)
        {
            PT63XX_WriteBufferOneBit(j, i, 1);
        }
    }
    PT63XX_ShowFrame();

    // 八段亮度调整
    for (int i = 0; i <= 7; ++i)
    {
        PT63XX_SetScreen(1, i);
        delay(1000);
    }

    PT63XX_ClearAll();
}

CMD1_SCAN_MODE PT63XX::getScanModeCMD(uint8_t gridNum)
{
    if (gridNum <= screenMaxGrid)
        return (CMD1_SCAN_MODE)(gridNum - 4);
    else
    {
        Serial.println("Scan mode set err");
        return (CMD1_SCAN_MODE)0b00000000;
    }
}

uint8_t PT63XX::getSetMemToCMD(uint8_t memIndex)
{
    if (memIndex <= (3 * screenMaxGrid - 1))
        return (0b11000000 | memIndex);
    else
    {
        Serial.println("Can not set addr to " + memIndex);
        return 0b00000000;
    }
}