#include <math.h>
#include <stdint.h>
#include "stm32f37x.h"
#include "USB_Class.h"
#include "Main.h"
//#include "usb_pwr.h"
#include <string.h>
// Interface functions =================================================================================================
struct {
  uint16_t LEDs_A;
  uint16_t LEDs_B;
  uint8_t  Buzzer;
} User;
enum   SYSSTAT {BTN1CLICK = 32, BTN1LCLICK = 16, BTN2CLICK = 8, BTN2LCLICK = 4, CHARGING = 2, EXTERNAL = 1};
//static uint16_t LEDs_A = 0, LEDs_B = 0;
//static uint8_t  Buzzer_Ctn = 0
uint8_t Btn1 = 0, Btn2 = 0, BtnsEvt = 0;
static uint8_t  SysStatus[2]; //B0[5]   - Btn1 click
                              //B0[4]   - Btn1 long click
                              //B0[3]   - Btn2 click
                              //B0[2]   - Btn2 long click
                              //B0[1]   - Charging
                              //B0[0]   - External power
                              //B1      - Battery level (0..100)
//----------------------------------------------------------------------------------------------------------------------
void SetBeep(uint8_t duration) {
  User.Buzzer = duration;
}
void Power(bool power) {
  if(power)
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
  else 
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}
void ReadSysStat() {
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
  int32_t Value = 0;
  for(uint8_t i = 0; i < 128; i++) {
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    Value += ADC_GetConversionValue(ADC1);
  }
  float Voltage = (4.52 * Value) / (4096.0 * 128.0);
  float Charge  = -3021687.0 + 4015244.0 * Voltage - 2129047.0 * pow(Voltage, 2.0) + 563081.3 * pow(Voltage, 3.0) - 74280.3 * pow(Voltage, 4.0) + 3910.256 * pow(Voltage, 5.0);
  if(Charge < 0.0)   Charge = 0.0;
  if(Charge > 100.0) Charge = 100.0;
  SysStatus[1] = Charge;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == Bit_SET) {
    SysStatus[0] |= EXTERNAL;
  } else {
    SysStatus[0] &= ~EXTERNAL;
  }
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == Bit_SET) {
    SysStatus[0] |= CHARGING;
  } else {
    SysStatus[0] &= ~CHARGING;
  }
}
void Buttons_Handle() {
  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_RESET) {
    if(Btn1 != 0xFF) Btn1++;
  } else {
    if(Btn1 == 0xFF) 
      SysStatus[0] |= BTN1LCLICK;
    else if(Btn1 > 20) 
      SysStatus[0] |= BTN1CLICK;
    Btn1 = 0;
  }
  if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) == Bit_RESET) {
    if(Btn2 != 0xFF) Btn2++;
  } else {
    if(Btn2 == 0xFF) 
      SysStatus[0] |= BTN2LCLICK;
    else if(Btn2 > 20) 
      SysStatus[0] |= BTN2CLICK;
    Btn2 = 0;
  }
}
void SetLeds(uint16_t row1, uint16_t row2) {
  User.LEDs_A = row1;
  User.LEDs_B = row2;
}
void Beep_Handle() {
  if(User.Buzzer == 0) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
  } else {
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    User.Buzzer--;
  }
}
void LEDs_Handle() {
  uint16_t Row;
  GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15);
  GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
  GPIO_ResetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7);
  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_RESET) {
    GPIO_ResetBits(GPIOC, GPIO_Pin_14);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    Row = User.LEDs_A;
  } else {
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    GPIO_SetBits(GPIOC, GPIO_Pin_14);
    Row = User.LEDs_B;
  }
  if(Row & 0x0001) GPIO_SetBits(GPIOA, GPIO_Pin_8);
  if(Row & 0x0002) GPIO_SetBits(GPIOA, GPIO_Pin_9);
  if(Row & 0x0004) GPIO_SetBits(GPIOA, GPIO_Pin_10);
  if(Row & 0x0008) GPIO_SetBits(GPIOF, GPIO_Pin_6);
  if(Row & 0x0010) GPIO_SetBits(GPIOF, GPIO_Pin_7);
  if(Row & 0x0020) GPIO_SetBits(GPIOA, GPIO_Pin_15);
  if(Row & 0x0040) GPIO_SetBits(GPIOB, GPIO_Pin_3);
  if(Row & 0x0080) GPIO_SetBits(GPIOB, GPIO_Pin_4);
  if(Row & 0x0100) GPIO_SetBits(GPIOB, GPIO_Pin_5);
  if(Row & 0x0200) GPIO_SetBits(GPIOB, GPIO_Pin_6);
  if(Row & 0x0400) GPIO_SetBits(GPIOB, GPIO_Pin_7);
  if(Row & 0x0800) GPIO_SetBits(GPIOB, GPIO_Pin_8);
  if(Row & 0x1000) GPIO_SetBits(GPIOB, GPIO_Pin_9);
  if(Row & 0x2000) {
    SysStatus[0] &= ~(BTN1LCLICK | BTN1CLICK | BTN2CLICK);
    Row &= 0x1FFF;
  }
}
// Time functions ======================================================================================================
static uint64_t MS_Ticks = 0;
//----------------------------------------------------------------------------------------------------------------------
void SysTick_Handler(void) {
  __disable_irq();
  MS_Ticks++;
  if(MS_Ticks & 0x0F) {
    LEDs_Handle();
    Beep_Handle();
    Buttons_Handle();
  }
  __enable_irq();
}
uint64_t GetTicks() {
  uint64_t Ticks;
  __disable_irq();
  Ticks = MS_Ticks;
  __enable_irq();
  return Ticks;
}
void SleepMS(uint16_t duration) {
  uint64_t End = GetTicks() + duration;
  while(MS_Ticks < End);
}
// Hardware init =======================================================================================================
//LEDs(PA8 PA9 PA10 PF6 PF7 PA15 PB3 PB4 PB5 PB6 PB7 PB8 PB9 - Rows PC13 PC14) Buzzer(PA0) NonoPI(PA2) ExtPower(PB2) BatCharge(PA3) BatLevel(PA1) Buttons(PD8 PC15)
void Interface_Config() {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOF, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  SysTick_Config(SystemCoreClock / 1000);
  //Inicio ADC para bateria
  ADC_InitTypeDef ADC_init_structure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_init_structure.ADC_ContinuousConvMode = DISABLE;
  ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_init_structure.ADC_ScanConvMode = DISABLE;
  ADC_Init(ADC1, &ADC_init_structure);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
}
// Capture functions ---------------------------------------------------------------------------------------------------
int16_t   INBuff[INBUFFER_LEN]; //Buffer de muestras
uint16_t  INReadPos;
uint16_t  INWritePos;
uint16_t  INBuffUsed;
void Buffers_Reset() {
  INReadPos   = 0;
  INWritePos  = 0;
  INBuffUsed  = 0;
}
void TIM2_IRQHandler() {
  GPIO_SetBits(GPIOA, GPIO_Pin_4);                                    /* SPI disable */
  if(INBuffUsed < INBUFFER_LEN) {
    INBuff[INWritePos]     = SDADC_GetConversionValue(SDADC3);
    INBuff[INWritePos + 1] = (SPI_I2S_ReceiveData16(SPI1) - 4096) * 8;
    if((INWritePos + 2) == INBUFFER_LEN) {
      INWritePos = 0;
    } else {
      INWritePos += 2;
    }
    INBuffUsed += 2;  
  } else {
    Buffers_Reset();
  }
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                                  /* SPI enable to low and setup ADC channel */
  SDADC_ClearITPendingBit(SDADC3, SDADC_IT_JEOC | SDADC_IT_EOCAL | SDADC_IT_JOVR | SDADC_IT_ROVR);
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  SPI_I2S_SendData16(SPI1, 0xFFFF);                                   /* Load data to transfer by SPI */
  SDADC_SoftwareStartConv(SDADC3);
}
bool Buffers_ToPMA(uint16_t dst) {
  if(INBuffUsed == 0) return FALSE;
  int16_t *Ptr    = (int16_t *)(dst * 2 + PMAAddr);
  uint8_t  Remain = 32;
  while(Remain) {
    if(INBuffUsed) {
      Ptr[0] = INBuff[INReadPos];
      Ptr[2] = INBuff[INReadPos + 1];
      if((INReadPos + 2) == INBUFFER_LEN) {
        INReadPos = 0;
      } else {
        INReadPos += 2;
      }
      INBuffUsed -= 2;
    } else {
      Ptr[0] = -32768;
      Ptr[2] = -32768;
    }
    Remain -= 2;
    Ptr = &Ptr[4];
  }
  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------
//Inicia el bus SPI (Solo si se lee del Infineon)
void Init_SPI() {
  SPI_InitTypeDef         SPI_InitStructure; 
  GPIO_InitTypeDef        GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                 /* Clock used devices */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                           /* CS pin */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                       /* Alternate function */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      /* Push-pull */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   /* Pin max speed 50MHz */
  GPIO_Init(GPIOA, &GPIO_InitStructure);                              /* Config pin */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; /* Configuring SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        /* Alternate function */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        /* Pull up enabled */
  GPIO_Init(GPIOA, &GPIO_InitStructure);                              /* Config pins */

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);                /* Select SPI1 Alternate function*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  /* Read only SPI */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       /* Master mode SPI */
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;                  /* 16 bits word size */
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                          /* Clk idle low */
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                        /* First transition is first bit */
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                           /* Software slave management */
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  /* Byte order */
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; /* SPI prescaler */
  SPI_InitStructure.SPI_CRCPolynomial = 7;                            /* SPI CRC polynomial */
  SPI_Init(SPI1, &SPI_InitStructure);                                 /* Init SPI2 */
  SPI_CalculateCRC(SPI1, DISABLE);                                    /* Disable CRC */
  SPI_Cmd(SPI1, ENABLE);                                              /* Enable SPI2 */
}
//Inicia el ADC usado para tension
void Init_SDADC() {
  SDADC_InitTypeDef       SDADC_InitStruct;
  SDADC_AINStructTypeDef  SDADC_AINStructure;
  GPIO_InitTypeDef        GPIO_InitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_SDADCAnalogCmd(PWR_SDADCAnalog_1 | PWR_SDADCAnalog_2 | PWR_SDADCAnalog_3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDADC1 | RCC_APB2Periph_SDADC2 | RCC_APB2Periph_SDADC3, ENABLE);
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div6);                      /* Set ADC prescaler (72MHz / 12 -> 6MHz) */
  SDADC_VREFSelect(SDADC_VREF_Ext);                                  /* External reference */
//  SDADC_Cmd(SDADC1, ENABLE);
//  SDADC_Cmd(SDADC2, ENABLE);
  SDADC_Cmd(SDADC3, ENABLE);
//  while(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_STABIP) == RESET);
//  while(SDADC_GetFlagStatus(SDADC2, SDADC_FLAG_STABIP) == RESET);
  while(SDADC_GetFlagStatus(SDADC3, SDADC_FLAG_STABIP) == RESET);

  //  SDADC_InitModeCmd(SDADC1, ENABLE);                                  /* Enter initialization mode */
  SDADC_InitModeCmd(SDADC3, ENABLE);
//  while(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_INITRDY) == RESET);
  while(SDADC_GetFlagStatus(SDADC3, SDADC_FLAG_INITRDY) == RESET);

//  SDADC_InitStruct.SDADC_Channel = SDADC_Channel_6;
  SDADC_InitStruct.SDADC_ContinuousConvMode = DISABLE;
  SDADC_InitStruct.SDADC_FastConversionMode = DISABLE;
//  SDADC_Init(SDADC1, &SDADC_InitStruct);
  SDADC_InitStruct.SDADC_Channel = SDADC_Channel_8;
  SDADC_Init(SDADC3, &SDADC_InitStruct);

//  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_Diff;/* Configure Analog input */
  SDADC_AINStructure.SDADC_Gain = SDADC_Gain_1_2;
  SDADC_AINStructure.SDADC_CommonMode = SDADC_CommonMode_VSSA;
  SDADC_AINStructure.SDADC_Offset = 0;
//  SDADC_AINInit(SDADC1, SDADC_Conf_0, &SDADC_AINStructure);
  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_Diff;/* Configure Analog input */
  SDADC_AINInit(SDADC3, SDADC_Conf_0, &SDADC_AINStructure);
  
//  SDADC_ChannelConfig(SDADC1, SDADC_Channel_6, SDADC_Conf_0);         /* Init ADC input configuration */
  SDADC_ChannelConfig(SDADC3, SDADC_Channel_8, SDADC_Conf_0);         /* Init ADC input configuration */
  SDADC_RegularSynchroSDADC1(SDADC3, ENABLE);

//  SDADC_InitModeCmd(SDADC1, DISABLE);
  SDADC_InitModeCmd(SDADC3, DISABLE);
//  SDADC_CalibrationSequenceConfig(SDADC1, SDADC_CalibrationSequence_1);
  SDADC_CalibrationSequenceConfig(SDADC3, SDADC_CalibrationSequence_1);
//  SDADC_StartCalibration(SDADC1);
  SDADC_StartCalibration(SDADC3);
//  while(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_EOCAL) == RESET);
  while(SDADC_GetFlagStatus(SDADC3, SDADC_FLAG_EOCAL) == RESET);
//  SDADC_SoftwareStartConv(SDADC1);
  SDADC_SoftwareStartConv(SDADC3);
}
void Init_Timer() {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                /* Set clock source for TIM2 */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                            /* Timer prescaler -> 1uS */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;         /* Count mode */
  TIM_TimeBaseStructure.TIM_Period = 4607;                            /* Reset period (16kHz) */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             /* */
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                    /* */
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                     /* Configure TIM2 */
  TIM_Cmd(TIM2, ENABLE);                                              /* Enable TIM2 */ 
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                          /* Enable overflow interrupt */

//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                     /* Interruption source */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;           /* Priority */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  /* Subpriority */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* Enable */
  NVIC_Init(&NVIC_InitStructure);                                     /* Configure TIM2 interrupt */
}
void Init_USB_HW() {
  NVIC_InitTypeDef        NVIC_InitStructure;         
  GPIO_InitTypeDef        GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;                   /* Interruption source */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           /* Priority */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  /* Subpriority */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* Enable */
  NVIC_Init(&NVIC_InitStructure);                                     /* Configure USB interrupt */
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;            /* Configure USB pins */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   /* High speed */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        /* Alternate function */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      /* Push-Pull */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                    /* */
  GPIO_Init(GPIOA, &GPIO_InitStructure);                              /* Set configuration */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_14);              /* Usb pins alternate function */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_14);
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);                    /* Select USBCLK source */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);                 /* Enable the USB clock */
	USB_Init();
}
// Main function -------------------------------------------------------------------------------------------------------
enum State_t {REBOOT, INIT, BOOTING, RUNNING, SHUTTINGDOWN, OFF};
uint8_t  State = REBOOT;
uint8_t *USB_SetStatus(uint16_t Length) {
  if(Length == 0) {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(User);
    return NULL;
  }
  return (uint8_t*)&User;
}
uint8_t *USB_GetStatus(uint16_t Length) {
  if(Length == 0) {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(SysStatus);
    return NULL;
  }
  return SysStatus;
}
void USB_ReadButtons() {
  SysStatus[0] &= ~(BTN1CLICK | BTN1LCLICK | BTN2CLICK);
}
int main(void) {
  Interface_Config();
  Init_SDADC();
  Init_SPI();
  Init_Timer();
  Init_USB_HW();
  uint8_t Counter;
  while(1) {
    ReadSysStat();
    if((SysStatus[0] & BTN2LCLICK) == BTN2LCLICK) {
      if(State == OFF) {
        State = REBOOT;
      } else {
        State = OFF;
      }
      SetBeep(80);
      SysStatus[0] &= ~BTN2LCLICK;
    }
    if(State == REBOOT) {
      Power(FALSE);
      SleepMS(1000);
      State = INIT;
    } else if(State == INIT) {
      Power(TRUE);
      State = BOOTING;
      Counter = 0;
    } else if((State == BOOTING)) {
      if(Counter < 13) {
        SetBeep(10);
        SetLeds(1 << Counter, 0);
        SleepMS(1000);
        SetLeds(0, 0);
        SleepMS(1000);
        Counter++;
        if(bDeviceState == CONFIGURED) {
          SetBeep(80);
          SetLeds(0, 0);
          State = RUNNING;
        }
      } else {
        State = REBOOT;
      }
    } else if(State == RUNNING) {
      
    } else if(State == OFF) {
      Power(FALSE);
      SetLeds(0, 0);
    } else if(State == SHUTTINGDOWN) {
      
    }
  }
}