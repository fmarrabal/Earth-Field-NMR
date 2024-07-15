#include "Main.h"
// ---------------------------------------------------------------------------------------------------------------------
static          uint8_t  PreDelay;
static          uint8_t  PostDelay;
static          uint16_t PulseLen;
static          uint16_t RecoveryLen;
static          uint32_t Samples;
static volatile uint16_t Scans;
// ---------------------------------------------------------------------------------------------------------------------
uint8_t Int2Hex(const char ch) {                                                //Convert binary to hex character
  return (ch < 10) ? ch + '0' : 'A' + ch - 10;                                  //
}                                                                               //
// --------------------------------------------------------------------------------------------------------------------
void SetTimes(const uint16_t pulse, const uint16_t recovery) {
  if(Scans != 0) return;
  PulseLen = pulse;
  RecoveryLen = recovery;
}
void SetDelays(const uint16_t predelay, const uint16_t posdelay) {
  if(Scans != 0) return;
  PreDelay = predelay;
  PostDelay = posdelay;
}
void SetSamples(const uint32_t samples) {
  if(Scans != 0) return;
  Samples = samples;
}
void Capture(const uint16_t scans) {
  if(Scans != 0) return;
  BUFF_Init();                                                              //Clear buffer
  Scans = scans;
}
// --------------------------------------------------------------------------------------------------------------------
void TIM6Delay(const uint16_t ms) {                                             //Basic delay
  TIM6->CNT = 0;                                                                //reset count
  TIM6->ARR = ms << 1;                                                          //
  TIM6->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;                                        //Start timer
  while ((TIM6->CR1 & TIM_CR1_CEN) != 0);                                         //Wait
  TIM6->SR &= ~TIM_SR_UIF;                                                      //Clear interrupt
}                                                                               //
// --------------------------------------------------------------------------------------------------------------------
void Main() {                                                                   //Entry point
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;                                           //Enable used periperals
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;                                            //
  GPIOA->MODER |= (0b01<<GPIO_MODER_MODER2_Pos)|(0b01<<GPIO_MODER_MODER3_Pos);  //Configure ports
  TIM6->PSC = (AHB_CLK / 2000) - 1;                                             //Tim 6 prescaler (0.5ms ticks)
  TIM6Delay(200);
  BUFF_Init();                                                                  //Init buffer
  SDADC_Init();                                                                 //Init ADC
  USB_Init();                                                                   //Init USB
  while(true) {                                                                 //repeat forever
    if(Scans != 0) {                                                            //If pending scans
      GPIOA->BSRR = GPIO_BSRR_BS_3;                                             //Enable polarize coil
      TIM6Delay(PulseLen);                                                      //Wait
      GPIOA->BSRR = GPIO_BSRR_BR_3;                                             //Disable polarize coil
      TIM6Delay(PreDelay);                                                      //Wait
      GPIOA->BSRR = GPIO_BSRR_BS_2;                                             //Enable relay
      TIM6Delay(PostDelay);                                                     //Wait
      SDADC_Capture(Samples);                                                   //Start capture
      GPIOA->BSRR = GPIO_BSRR_BR_2;                                             //Disable relay
      TIM6Delay(RecoveryLen);                                                   //Wait
      Scans--;                                                                  //
      BUFF_IncScan();                                                           //Move to next scan
    }                                                                           //
  }                                                                             //
}                                                                               //
// --------------------------------------------------------------------------------------------------------------------