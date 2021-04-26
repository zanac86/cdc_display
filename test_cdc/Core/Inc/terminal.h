#ifndef TERMINAL_H
#define TERMINAL_H

#include "main.h"
#include <string.h>
#include <stdint.h>
#include "OLED.h"

extern volatile uint8_t received;

void TERM_Reset();

void TERM_Rece(const uint8_t* Buf, uint32_t* Len);

void TERM_Display();

void TERM_Rece_From_Rx_Buffer();

#endif
