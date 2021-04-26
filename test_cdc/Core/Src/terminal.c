#include "terminal.h"
#include <stdbool.h>

#include "usbd_cdc_if.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

// 32px / 8px = 4
// но массив сделать на одну строку больше,
// чтобы последний 0a не оставлял пустую последнюю строку на экране
#define MaxLines 5

// 128px / 6px = 21 +1 for \0
#define MaxChars 21

char lines[MaxLines][MaxChars + 1];

uint8_t currentLine;

// cursor position

typedef struct
{
    uint8_t x;
    uint8_t y;
} Cursor;

Cursor pos;
Cursor old;

void TERM_Reset()
{
    pos.x       = 0;
    pos.y       = 0;
    old         = pos;
    received    = 0;
    currentLine = 0;
    for (int i = 0; i < MaxLines; i++)
    {
        memset(lines[i], 1, MaxChars + 1);
    }
}

bool isPrintable(uint8_t c)
{
    if ((c >= 32) && (c != 127))
        return true;
    return false;
}

void scrollUp()
{
    for (int i = 0; i < (MaxLines - 1); i++)
    {
        memcpy(lines[i], lines[i + 1], MaxChars + 1);
    }
    memset(lines[MaxLines - 1], 0, MaxChars + 1); // 186 middle dot
}

void nextLine()
{
    pos.y++;
    if (pos.y == MaxLines)
    {
        scrollUp();
        pos.y = (MaxLines - 1);
    }
}

void TERM_Rece(const uint8_t* Buf, uint32_t* Len)
{
    if (*Len == 0)
    {
        return;
    }
    for (int i = 0; i < (*Len); i++)
    {
        char c = Buf[i];
        if (c == '#')
        {
            TERM_Reset();
            break;
        }
        if (c == 0x0a)
        {
            //        	lines[pos.y][pos.x]='\0';
            nextLine();
            continue;
        }
        if (c == 0x0d)
        {
            pos.x = 0;
            continue;
        }
        if (pos.x < MaxChars)
        {
            lines[pos.y][pos.x] = c;
            pos.x++;
        }
    }
}

void TERM_Rece_From_Rx_Buffer()
{
    USBD_CDC_HandleTypeDef* hcdc = (USBD_CDC_HandleTypeDef*)(hUsbDeviceFS.pClassData);
    const uint8_t* Buf           = hcdc->RxBuffer;
    uint32_t Len                 = hcdc->RxLength;
    TERM_Rece(Buf, &Len);
}

void TERM_Display()
{
    OLED_Clear(0);
    // реально в массиве на одну строку больше чем на экране
    for (int i = 0; i < (MaxLines - 1); i++)
    {
        lines[i][MaxChars] = '\0';
        OLED_FastDrawStr(lines[i], 0, i, 0);
    }
    OLED_UpdateScreen();
}
