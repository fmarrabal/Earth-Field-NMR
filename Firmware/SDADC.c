#include "Main.h"
// ---------------------------------------------------------------------------------------------------------------------
static uint16_t          Buffer[64];                                            //SDADC1 buffer
static volatile uint32_t Samples;                                               //Pending samples to capture
// ---------------------------------------------------------------------------------------------------------------------
void DMA2_CH3_ISR() {                                                           //Handle DMA interrupt
  uint16_t *Ptr;                                                                //
  if((DMA2->ISR & DMA_ISR_TCIF3) != 0) {                                        //Prepare Trasnfer complete
    DMA2->IFCR |= DMA_IFCR_CTCIF3;                                              //Interrupt clear
    Ptr = &Buffer[32];                                                          //Init pointer
  } else {                                                                      //Prepare half transfer
    DMA2->IFCR |= DMA_IFCR_CHTIF3;                                              //Interrupt clear
    Ptr = &Buffer[0];                                                           //Init pointer
  }                                                                             //
  if(Samples == 0) {                                                            //If no more samples
    SDADC1->CR2 &= ~SDADC_CR2_RCONT;                                            //Disable SDADC
    DMA2_Channel3->CCR = 0;                                                     //Reset DMA
  } else {                                                                      //
    for(uint8_t n = 0; n < 32; n++) BUFF_PushSample(*Ptr++);                    //Copy sample
    Samples = Samples - 32;                                                     //Decrement counter
  }                                                                             //
}                                                                               //
void SDADC_Capture(const uint32_t samples) {                                    //Init capture
  Samples = samples & 0xFFFFFFE0;                                               //Round to 32
  DMA2->IFCR |= DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3;                              //Interrupt clear
  DMA2_Channel3->CPAR = (uint32_t)&SDADC1->RDATAR;                              //Initialize SDADC1 DMA controller
  DMA2_Channel3->CMAR = (uint32_t)Buffer;                                       //
  DMA2_Channel3->CNDTR = 64;                                                    //
  DMA2_Channel3->CCR = DMA_CCR_MSIZE_0|DMA_CCR_PSIZE_0|DMA_CCR_EN |             //
                       DMA_CCR_CIRC|DMA_CCR_MINC|DMA_CCR_TCIE|DMA_CCR_HTIE;     //
  SDADC1->CR2 |= SDADC_CR2_RCONT | SDADC_CR2_RSWSTART;                          //Start conversion
  while(Samples != 0);                                                          //Wait for ADC to complete
}                                                                               //
void SDADC_Init() {                                                             //Init ADCs capture
  RCC->AHBENR |=  RCC_AHBENR_DMA2EN  | RCC_AHBENR_GPIOEEN;                      //Enable used devices in AHB
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                                            //
  PWR->CR |= PWR_CR_SDADC1EN;                                                   //Enable SDADC power
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN | RCC_APB2ENR_SDADC1EN;                  //Enable OPAMPs and used timers
  GPIOE->MODER |= (0b11<<GPIO_MODER_MODER8_Pos)|(0b11<<GPIO_MODER_MODER9_Pos);  //PE8=AN, PE9=AN
  SDADC1->CR2 |= SDADC_CR2_ADON;                                                //Enable SDADC
  while((SDADC1->ISR & SDADC_ISR_STABIP) != 0);                                 //Wait for stabilization
  SDADC1->CR1 |= SDADC_CR1_INIT;                                                //Initialization
  while((SDADC1->ISR & SDADC_ISR_INITRDY) == 0);                                //Wait initialization bit to be set
  SDADC1->CR1 |= SDADC_CR1_RDMAEN;                                              //Enable DMA
  SDADC1->CR2 |= SDADC_CR2_FAST | (0b1000 << SDADC_CR2_RCH_Pos);                //Config capture channel and mode
  SDADC1->CR1 &= ~SDADC_CR1_INIT;                                               //Exit initialization mode
  SDADC1->CR2 |= SDADC_CR2_STARTCALIB;                                          //Start calibration
  while((SDADC1->ISR & SDADC_ISR_CALIBIP) != 0);                                //Wait calibration to finish
  NVIC_SetPriority(DMA2_Channel3_IRQn, 0);                                      //Set interrupts priority and enable then
  NVIC_EnableIRQ(DMA2_Channel3_IRQn);                                           //

//  ADC12_COMMON->CCR = ADC12_CCR_CKMODE_0 | ADC12_CCR_MDMA_1 |                   //Configure dual mode and clock source
//                      ADC12_CCR_DMACFG | ADC12_CCR_MULTI_0;                     //
//  ADC34_COMMON->CCR = ADC34_CCR_CKMODE_0 | ADC34_CCR_MDMA_1 |                   //
//                      ADC34_CCR_DMACFG | ADC34_CCR_MULTI_0;                     //
//  ADC1->CR = 0;                                                                 //Enable ADCs VREF (First step)
//  ADC1->CR = ADC_CR_ADVREGEN_0;                                                 //
//  ADC2->CR = 0;                                                                 //
//  ADC2->CR = ADC_CR_ADVREGEN_0;                                                 //
//  ADC3->CR = 0;                                                                 //
//  ADC3->CR = ADC_CR_ADVREGEN_0;                                                 //
//  ADC4->CR = 0;                                                                 //
//  ADC4->CR = ADC_CR_ADVREGEN_0;                                                 //
//  SystickDelay(1000);                                                           //Wait for ADCs VREF to stabilize


//  NVIC_EnableIRQ(DMA1_Channel1_IRQn);                                           //
//  ADC1->CR |= ADC_CR_ADCALDIF | ADC_CR_ADCAL;                                   //Calibrate ADC1 (Differential)
//  while(ADC1->CR & ADC_CR_ADCAL);                                               //Wait for calibration to ends
//  ADC2->CR |= ADC_CR_ADCALDIF | ADC_CR_ADCAL;                                   //Calibrate ADC2 (Differential)
//  while(ADC2->CR & ADC_CR_ADCAL);                                               //Wait for calibration to ends
//  ADC3->CR |= ADC_CR_ADCALDIF | ADC_CR_ADCAL;                                   //Calibrate ADC3 (Differential)
//  while(ADC3->CR & ADC_CR_ADCAL);                                               //Wait for calibration to ends
//  ADC4->CR |= ADC_CR_ADCAL;                                                     //Calibrate ADC4 (Single ended)
//  while(ADC4->CR & ADC_CR_ADCAL);                                               //Wait for calibration to ends
//  ADC4->CR |= ADC_CR_ADCALDIF | ADC_CR_ADCAL;                                   //Calibrate ADC4 (Differential)
//  while(ADC4->CR & ADC_CR_ADCAL);                                               //Wait for calibration to ends
//  ADC1->DIFSEL = ADC_DIFSEL_DIFSEL_7;                                           //Configure differential channels
//  ADC2->DIFSEL = ADC_DIFSEL_DIFSEL_5 | ADC_DIFSEL_DIFSEL_2;                     //
//  ADC3->DIFSEL = ADC_DIFSEL_DIFSEL_14 | ADC_DIFSEL_DIFSEL_7;                    //
//  ADC4->DIFSEL = ADC_DIFSEL_DIFSEL_9 | ADC_DIFSEL_DIFSEL_0;                     //
//  ADC1->CR |= ADC_CR_ADEN;                                                      //Enable ADC1
//  while((ADC1->ISR & ADC_ISR_ADRDY) == 0);                                      //Wait for ADC to be ready
//  ADC2->CR |= ADC_CR_ADEN;                                                      //Enable ADC2
//  while((ADC2->ISR & ADC_ISR_ADRDY) == 0);                                      //Wait for ADC to be ready
//  ADC3->CR |= ADC_CR_ADEN;                                                      //Enable ADC3
//  while((ADC3->ISR & ADC_ISR_ADRDY) == 0);                                      //Wait for ADC to be ready
//  ADC4->CR |= ADC_CR_ADEN;                                                      //Enable ADC4
//  while((ADC4->ISR & ADC_ISR_ADRDY) == 0);                                      //Wait for ADC to be ready
//  ADC1->SMPR1 = (0b100 << ADC_SMPR1_SMP9_Pos)  | (0b100 << ADC_SMPR1_SMP8_Pos) |//Set uset channels sample time
//                (0b100 << ADC_SMPR1_SMP3_Pos);                                  //to 19.5 ADC cycles
//  ADC2->SMPR1 = (0b100 << ADC_SMPR1_SMP3_Pos)  | (0b100 << ADC_SMPR1_SMP4_Pos) |//Total 32cyles per sample
//                (0b100 << ADC_SMPR1_SMP6_Pos)  | (0b100 << ADC_SMPR1_SMP7_Pos); //
//  ADC3->SMPR1 = (0b100 << ADC_SMPR1_SMP8_Pos)  | (0b100 << ADC_SMPR1_SMP9_Pos); //
//  ADC3->SMPR2 = (0b100 << ADC_SMPR2_SMP15_Pos) | (0b100 << ADC_SMPR2_SMP16_Pos);//
//  ADC4->SMPR1 = (0b100 << ADC_SMPR1_SMP1_Pos)  | (0b100 << ADC_SMPR1_SMP2_Pos); //
//  ADC4->SMPR2 = (0b100 << ADC_SMPR2_SMP10_Pos) | (0b100 << ADC_SMPR2_SMP11_Pos);//
//  ADC1->SQR1  = 15 | (8 << ADC_SQR1_SQ1_Pos) | (3 << ADC_SQR1_SQ2_Pos) |        //Configure capture sequences
//                     (8 << ADC_SQR1_SQ3_Pos) | (3 << ADC_SQR1_SQ4_Pos);         //
//  ADC1->SQR2  = (8 << ADC_SQR2_SQ5_Pos) | (3 << ADC_SQR2_SQ6_Pos) |             //
//                (8 << ADC_SQR2_SQ7_Pos) | (3 << ADC_SQR2_SQ8_Pos) |             //
//                (8 << ADC_SQR2_SQ9_Pos);                                        //
//  ADC1->SQR3 =  (3 << ADC_SQR3_SQ10_Pos) | (8 << ADC_SQR3_SQ11_Pos) |           //
//                (3 << ADC_SQR3_SQ12_Pos) | (8 << ADC_SQR3_SQ13_Pos) |           //
//                (3 << ADC_SQR3_SQ14_Pos);                                       //
//  ADC1->SQR4 = (8 << ADC_SQR4_SQ15_Pos) | (15 << ADC_SQR4_SQ16_Pos);            //
//  ADC2->SQR1  = 15 | (6 << ADC_SQR1_SQ1_Pos) | (3 << ADC_SQR1_SQ2_Pos) |        //
//                     (6 << ADC_SQR1_SQ3_Pos) | (3 << ADC_SQR1_SQ4_Pos);         //
//  ADC2->SQR2 = (6 << ADC_SQR2_SQ5_Pos) | (3 << ADC_SQR2_SQ6_Pos) |              //
//               (6 << ADC_SQR2_SQ7_Pos) | (3 << ADC_SQR2_SQ8_Pos) |              //
//               (6 << ADC_SQR2_SQ9_Pos);                                         //
//  ADC2->SQR3 = (3 << ADC_SQR3_SQ10_Pos) | (6 << ADC_SQR3_SQ11_Pos) |            //
//               (3 << ADC_SQR3_SQ12_Pos) | (6 << ADC_SQR3_SQ13_Pos) |            //
//               (3 << ADC_SQR3_SQ14_Pos);                                        //
//  ADC2->SQR4 = (6 << ADC_SQR4_SQ15_Pos) | (3 << ADC_SQR4_SQ16_Pos);             //
//  ADC3->SQR1  = 15 | (8 << ADC_SQR1_SQ1_Pos) | (15 << ADC_SQR1_SQ2_Pos) |       //
//                     (8 << ADC_SQR1_SQ3_Pos) | (15 << ADC_SQR1_SQ4_Pos);        //
//  ADC3->SQR2 = (8 << ADC_SQR2_SQ5_Pos) | (15 << ADC_SQR2_SQ6_Pos) |             //
//               (8 << ADC_SQR2_SQ7_Pos) | (15 << ADC_SQR2_SQ8_Pos) |             //
//               (8 << ADC_SQR2_SQ9_Pos);                                         //
//  ADC3->SQR3 = (15 << ADC_SQR3_SQ10_Pos) | (8 << ADC_SQR3_SQ11_Pos) |           //
//               (15 << ADC_SQR3_SQ12_Pos) | (8 << ADC_SQR3_SQ13_Pos) |           //
//               (15 << ADC_SQR3_SQ14_Pos);                                       //
//  ADC3->SQR4 = (8 << ADC_SQR4_SQ15_Pos) | (15 << ADC_SQR4_SQ16_Pos);            //
//  ADC4->SQR1  = 15 | (10 << ADC_SQR1_SQ1_Pos) | (1 << ADC_SQR1_SQ2_Pos) |       //
//                     (10 << ADC_SQR1_SQ3_Pos) | (1 << ADC_SQR1_SQ4_Pos);        //
//  ADC4->SQR2 = (10 << ADC_SQR2_SQ5_Pos) | (1 << ADC_SQR2_SQ6_Pos) |             //
//               (10 << ADC_SQR2_SQ7_Pos) | (1 << ADC_SQR2_SQ8_Pos) |             //
//               (10 << ADC_SQR2_SQ9_Pos);                                        //
//  ADC4->SQR3 = (1 << ADC_SQR3_SQ10_Pos) | (10 << ADC_SQR3_SQ11_Pos) |           //
//               (1 << ADC_SQR3_SQ12_Pos) | (10 << ADC_SQR3_SQ13_Pos) |           //
//               (1 << ADC_SQR3_SQ14_Pos);                                        //
//  ADC4->SQR4 = (10 << ADC_SQR4_SQ15_Pos) | (1 << ADC_SQR4_SQ16_Pos);            //
//  OPAMP1->CSR |= OPAMP_CSR_VMSEL_0 | OPAMP_CSR_VPSEL_Msk | OPAMP1_CSR_OPAMP1EN; //Configure and enable OPAMP1
//  ADC1->CFGR = ADC_CFGR_EXTEN_0 | (11 << ADC_CFGR_EXTSEL_Pos);                  //Enable TIM2_TRGO trigger for each conversion
//  ADC3->CFGR = ADC_CFGR_EXTEN_0 | (7 << ADC_CFGR_EXTSEL_Pos);                   //Enable TIM2_TRGO trigger for each conversion
//  ADC1->CR |= ADC_CR_ADSTART;                                                   //Enable ADC1 and ADC2
//  ADC3->CR |= ADC_CR_ADSTART;                                                   //Enable ADC3 and ADC4



}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
