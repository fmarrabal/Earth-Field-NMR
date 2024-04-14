#pragma once
#include "usb_lib.h"
#define INBUFFER_LEN   6400
bool Buffers_ToPMA(uint16_t dst);
uint8_t *USB_SetStatus(uint16_t Length);
uint8_t *USB_GetStatus(uint16_t Length);
void SetLeds(uint16_t row1, uint16_t row2);
void SetBeep(uint8_t duration);
void USB_ReadButtons();