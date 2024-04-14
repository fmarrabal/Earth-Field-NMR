/* ================================ openZmeter =========================================================================
 * File:     SDADC_Capture.h
 * Author:   Eduardo Viciana (2018)
 * ------------------------------------------------------------------------------------------------------------------ */
#include "Main.h"
#include <arm_math.h>
//Definiciones locales -------------------------------------------------------------------------------------------------
typedef struct {
  float A0;     //A2=A0 en low pass
  float A1;
  float B1;
  float B2;
} Filter_t;
typedef struct {
  uint16_t CurrentN;  //ADC1_IN4
  uint16_t CurrentR;  //ADC2_IN12
  uint16_t Battery;   //ADC1_IN1
  uint16_t VoltageR;  //ADC2_IN1
} ADCSample_ADC12;
typedef struct {
  uint16_t CurrentS;  //ADC3_IN5
  uint16_t CurrentT;  //ADC4_IN5
  uint16_t VoltageS;  //ADC3_IN12
  uint16_t VoltageT;  //ADC4_IN3
} ADCSample_ADC34;
typedef struct {
  uint8_t         ChannelMask;        //Mascara de canales en uso
  uint8_t         Oversample;         //Configuracion de oversample
  uint8_t         SampleCounter;      //contador de muestras procesadas
  uint8_t         Channels;           //Numero de canales activos
} CaptureConf_t;
//Variables locales ----------------------------------------------------------------------------------------------------
Filter_t        Filter;               //Almacena coeficientes de los filtros
ChannelConf_t   ChannelsConf[7];      //Almacena configuracion y valores de los filtros
CaptureConf_t   CaptureConf;          //Configuracion y estado de la captura
ADCSample_ADC12 ADC12_Buffer[32];     //Buffer de captura para DMA1
ADCSample_ADC34 ADC34_Buffer[32];     //Buffer de captura para DMA2
int16_t         INBuff[INBUFFER_LEN]; //Buffer de muestras
uint16_t        INReadPos;
uint16_t        INWritePos;
uint16_t        INBuffUsed;
uint8_t         StreamReset;          //Si es != 0 hay un reset del stream
//----------------------------------------------------------------------------------------------------------------------

/**
 * Configura todos los ADCs usados, timer necesario para la captura, controlador DMA e interrupciones
 */
void ADC_Configure() {
  //Activo los canales ADC y los puertos involucrados
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_ADC12 | RCC_AHBPeriph_ADC34 | RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  //Configuro los 7 puertos de entrada y el de bateria
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Configuracion de ADCs
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div1 | RCC_ADC34PLLCLK_Div1);
  //Configuro ADC
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
  ADC_VoltageRegulatorCmd(ADC2, ENABLE);
  ADC_VoltageRegulatorCmd(ADC3, ENABLE);
  ADC_VoltageRegulatorCmd(ADC4, ENABLE);
  Waitms(1);
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_RegSimul;
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;
  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC2, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC3, &ADC_CommonInitStructure);
  ADC_CommonInit(ADC4, &ADC_CommonInitStructure);
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_2;
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel = 2;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_Init(ADC4, &ADC_InitStructure);
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_SelectCalibrationMode(ADC2, ADC_CalibrationMode_Single);
  ADC_SelectCalibrationMode(ADC3, ADC_CalibrationMode_Single);
  ADC_SelectCalibrationMode(ADC4, ADC_CalibrationMode_Single);  
  ADC_StartCalibration(ADC1);
  ADC_StartCalibration(ADC2);
  ADC_StartCalibration(ADC3);
  ADC_StartCalibration(ADC4);
  while((ADC_GetCalibrationStatus(ADC1) != RESET) || (ADC_GetCalibrationStatus(ADC2) != RESET) || 
        (ADC_GetCalibrationStatus(ADC3) != RESET) || (ADC_GetCalibrationStatus(ADC4) != RESET));
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);
  ADC_Cmd(ADC4, ENABLE);
  while((ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY) == RESET) || (ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY) == RESET) || 
        (ADC_GetFlagStatus(ADC3, ADC_FLAG_RDY) == RESET) || (ADC_GetFlagStatus(ADC4, ADC_FLAG_RDY) == RESET));
  ADC_RegularChannelConfig(ADC1, ADC_InjectedChannel_4,  1, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_InjectedChannel_1,  2, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_InjectedChannel_12, 1, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_InjectedChannel_1,  2, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC3, ADC_InjectedChannel_5,  1, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC3, ADC_InjectedChannel_12, 2, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC4, ADC_InjectedChannel_5,  1, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC4, ADC_InjectedChannel_3,  2, ADC_SampleTime_1Cycles5);
  ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_DMAConfig(ADC3, ADC_DMAMode_Circular);
  ADC_DMACmd(ADC3, ENABLE);
  ADC_StartConversion(ADC1);
  ADC_StartConversion(ADC2);
  ADC_StartConversion(ADC3);
  ADC_StartConversion(ADC4);
  //Inicio DMA
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1_2->CDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC12_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 64;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC3_4->CDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC34_Buffer;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);
  DMA_Cmd(DMA2_Channel5, ENABLE);
  //Habilito interrupciones del DMA solo uso un canal, ya que todas las operaciones finalizan a la vez
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * Inicia la captura de muestras, iniciando los parametros de los filtros si son necesarios
 * @param channels mascara de bits que indica que canales capturar el orden es [xx Vr Cr Vs Cs Vt Ct Cn]
 * @param mode modo de captura, los dos bits mas significativos son el oversample (00=1x, 01=4x, 10=16x, 11=64x)
 *             los demas bits determinan la velocidad de captura
 * @return el valor devuelto es true si los parametros son validos y la captura se inicia correctamente
 */
const uint16_t ADC_TimerDivs[] = { 150,  180,  200,  225,  250,  300,  260,  375,  450,  500,  600,  625,  750,  900, 1000,  1125,  1250, 
                                  1500, 1800, 1875, 2250, 2500, 3000, 3125, 3750, 4500, 5000, 5625, 6250, 7500, 9000, 9375, 11250, 12500};
bool ADC_StartCapture(uint8_t channels, uint8_t decimation, uint16_t speed) {
  ADC_StopCapture();
  //Recuento de canales activos, copio parametros de ajuste y inicio estado
  CaptureConf.Channels = 0;
  for(uint8_t Mask = 0x40, In = 0; Mask != 0; Mask >>= 1, In++) {
    if(Mask & channels) {
      ChannelsConf[CaptureConf.Channels].Z1 = 0.0;
      ChannelsConf[CaptureConf.Channels].Z2 = 0.0;
      ChannelsConf[CaptureConf.Channels].Offset = -32768.0 + Offsets[In];
      CaptureConf.Channels++;
    }  
  }
  if((CaptureConf.Channels == 0) || (decimation > 63)) return FALSE;
  CaptureConf.ChannelMask = channels;
  CaptureConf.SampleCounter = 0;
  CaptureConf.Oversample = decimation;
  
//  uint32_t Freq = SystemCoreClock / ADC_TimerDivs[25 & 0x3F];
//  if((mode & 0xC0) == 0x40) Freq = Freq / 4;
//  if((mode & 0xC0) == 0x80) Freq = Freq / 16;
//  if((mode & 0xC0) == 0xC0) Freq = Freq / 64;
//  if(((Freq * CaptureConf.Channels) > 360000) || (Freq < 5000)) return FALSE;


  
  

  float Q = 1.0f / sqrt(2.0f);
  float Fc = 1.0f / (CaptureConf.Oversample + 1);
//  if(SampleCounter == 0xC0) Fc = Fc / 4.0;
//  if(SampleCounter == 0xC0) Fc = Fc / 16.0;
//  if(SampleCounter == 0xC0) Fc = Fc / 64.0;
  float K    = tan(PI * Fc);
  float Norm = 1 / (1 + K / Q + K * K);
  Filter.A0 = K * K * Norm;
  Filter.A1 = 2 * Filter.A0;
 // Filter.A2 = Filter.A0;
  Filter.B1 = 2 * (K * K - 1) * Norm;
  Filter.B2 = (1 - K / Q + K * K) * Norm;
  Buffers_Reset();
  //Configuro TIMER1 CC3 que es compartido por los 4 ADCs
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_Period = 480 - 1;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
  return TRUE;
}
/**
 * Detiene la captura
 */
void ADC_StopCapture() {
  TIM_Cmd(TIM1, DISABLE);
  Buffers_Reset();
}

/**
 * Filtra una muestra en de entrada segun los parametros configurados del filtro
 * @param in muestra de entrada a añadir al filtro
 * @param channel canal de la muestra
 */
void ADC_Filter(float in, ChannelConf_t *channel) {
  in = (in + channel->Offset);
  if(CaptureConf.Oversample == 0) {
    channel->Sample = in;
  } else {
    channel->Sample = in * Filter.A0 + channel->Z1;
    channel->Z1 = in * Filter.A1 + channel->Z2 - Filter.B1 * channel->Sample;
    channel->Z2 = in * Filter.A0 - Filter.B2 * channel->Sample;
  }
}

/**
 * Control de la interrupcion del DMA del ADC, se llama cada vez que tenemos 16
 * muestras para procesar (medio buffer). Los pasos en cada interrupcion son
 * determinar el trozo de buffer a procesar, pasar por el filtro los canales
 * activos, 
 * 
 */
void DMA1_Channel1_IRQHandler(void) {
  ADCSample_ADC12 *ADC12;
  ADCSample_ADC34 *ADC34;
  if(DMA_GetITStatus(DMA1_IT_TC1) != RESET) {
    ADC12 = &ADC12_Buffer[16];
    ADC34 = &ADC34_Buffer[16];
    DMA_ClearITPendingBit(DMA1_IT_TC1); 
  } else {
    ADC12 = &ADC12_Buffer[0];
    ADC34 = &ADC34_Buffer[0];
    DMA_ClearITPendingBit(DMA1_IT_HT1);
  }
  for(uint8_t PendSamples = 16; PendSamples > 0; PendSamples--) {
    ChannelConf_t *Ptr = ChannelsConf;
    if(CaptureConf.ChannelMask & 0x40) ADC_Filter(ADC12->VoltageR, Ptr++);
    if(CaptureConf.ChannelMask & 0x20) ADC_Filter(ADC12->CurrentR, Ptr++);
    if(CaptureConf.ChannelMask & 0x10) ADC_Filter(ADC34->VoltageS, Ptr++);
    if(CaptureConf.ChannelMask & 0x08) ADC_Filter(ADC34->CurrentS, Ptr++);
    if(CaptureConf.ChannelMask & 0x04) ADC_Filter(ADC34->VoltageT, Ptr++);
    if(CaptureConf.ChannelMask & 0x02) ADC_Filter(ADC34->CurrentT, Ptr++);
    if(CaptureConf.ChannelMask & 0x01) ADC_Filter(ADC12->CurrentN, Ptr++);
    if(CaptureConf.Oversample == CaptureConf.SampleCounter) {
      Buffer_AppendSamples();
      CaptureConf.SampleCounter = 0;
    } else {
      CaptureConf.SampleCounter++;
    }
  }
}

/**
 * Reinicia los contadores del buffer a sus valores por defecto
 */
void Buffers_Reset() {
  INReadPos   = 0;
  INWritePos  = 0;
  INBuffUsed  = 0;
  StreamReset = 1;
}

/**
 * Añade muestras al buffer de salida si hay espacio, en caso contrario reinicia el buffer previamente
 */
void Buffer_AppendSamples() {
  ChannelConf_t *Ptr = ChannelsConf;
  if(INBuffUsed >= (INBUFFER_LEN - CaptureConf.Channels)) Buffers_Reset();
  for(uint8_t Chan = CaptureConf.Channels; Chan > 0; Chan--, Ptr++) {
    if(Ptr->Sample < -32767) 
      INBuff[INWritePos] = -32767;
    else if(Ptr->Sample > 32767)
      INBuff[INWritePos] = 32767;
    else
       INBuff[INWritePos] = Ptr->Sample;
    INWritePos = (INWritePos + 1) % INBUFFER_LEN;
  }
  INBuffUsed = INBuffUsed + CaptureConf.Channels;
}

/**
 * Genera un paquete de datos para ser enviado por USB. El tamaño del paquete
 * es de 64 bytes donde entra un maximo de 32 muestras.
 *   Si no hay datos para enviar se devuelve FALSE
 *   Si el buffer esta en estado de reset la primera muestra es 0xFFFF, y se limpiara el estado de reset
 *   Si una muestra no se usa (el buffer tiene menos de 32 muestras) se usara el valor 0xFFFF para indicarlo
 * @param dst posicion de escritura del buffer (debe de tener 64 bytes)
 * @return TRUE si hay datos para enviar, FALSE en otro caso
 */
bool Buffers_ToPMA(uint16_t dst) {
  if(INBuffUsed == 0) return FALSE;
  int16_t *Ptr    = (int16_t *)(dst * 2 + PMAAddr);
  uint8_t   Remain = 32;
  if(StreamReset) {
    *Ptr = -32768;
    Remain--;
    Ptr = &Ptr[2];
    StreamReset = 0;
  }
  while(Remain) {
    if(INBuffUsed) {
      *Ptr = INBuff[INReadPos];
      INReadPos = (INReadPos + 1) % INBUFFER_LEN;
      INBuffUsed--;
    } else {
      *Ptr = -32768;
    }
    Remain--;
    Ptr = &Ptr[2];
  }
  return TRUE;
}