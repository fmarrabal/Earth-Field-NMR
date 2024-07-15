
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stm32f373xc.h>
//---------------------------------------------------------------------------------------------------------------------
#define AHB_CLK      72000000                                                   //AHB_Clock
//---------------------------------------------------------------------------------------------------------------------
uint8_t Int2Hex(const char ch);
void Main();
void SetTimes(const uint16_t pulse, const uint16_t recovery);
void SetDelays(const uint16_t predelay, const uint16_t posdelay);
void SetSamples(const uint32_t samples);
void Capture(const uint16_t scans);

void BUFF_Init();
void BUFF_FillPMA(uint32_t *buffer);
void BUFF_PushSample(const uint16_t value);
void BUFF_IncScan();

void SDADC_Init();
void SDADC_Capture(const uint32_t samples);
void DMA2_CH3_ISR();

void USB_Init();
void USB_LP_ISR();
//---------------------------------------------------------------------------------------------------------------------