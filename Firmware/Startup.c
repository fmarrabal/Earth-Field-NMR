#include "Main.h"
extern uint8_t DATA_VALUES, DATA_START, DATA_END, BSS_START, BSS_END, STACK_END;
// ---------------------------------------------------------------------------------------------------------------------
void Sys_Init() {                                                               //System basic initialization
  unsigned char *src  = &DATA_VALUES;                                           //Init data section
  unsigned char *dest = &DATA_START;                                            //
  unsigned len = &DATA_END - &DATA_START;                                       //
  while(len--) *dest++ = *src++;                                                //
  dest = &BSS_START;                                                            //Zero bss section
  len = &BSS_END - &BSS_START;                                                  //
  while(len--) *dest++ = 0;                                                     //
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));                                  //Enable FPU
  FLASH->ACR |= FLASH_ACR_PRFTBE | (0b10 << FLASH_ACR_LATENCY_Pos);             //FLASH prefetch enabled and 2 wait state
  RCC->CR |= RCC_CR_HSEON;                                                      //Enable HSE oscillator
  while((RCC->CR & RCC_CR_HSERDY) == 0);                                        //Wait for HSE become stable
  RCC->CFGR |= RCC_CFGR_PLLMUL9 | RCC_CFGR_PLLSRC | RCC_CFGR_PPRE1_2;           //Setup PLL source, multiplier and prescaler
  RCC->CR |= RCC_CR_PLLON;                                                      //Turn on PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0);                                        //Wait for PLL to lock
  RCC->CFGR |= (0b10 << RCC_CFGR_SW_Pos) | RCC_CFGR_SDADCPRE_DIV12;              //Set PLL as system clock
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                                            //Enable used peripherals in APB1
  NVIC_SetPriorityGrouping(0);                                                  //Set to 4 bits priorities and no subpriorities
  __enable_irq();                                                               //Enable interrupts
  Main();                                                                       //Call main
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
static void Default_Handler() {                                                 //Default interrupt handler
  __BKPT(0);                                                                    //Break into the debugger
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
__attribute__((used)) void HardFault_HandlerC(unsigned long *hardfault_args) {
  volatile unsigned long stacked_r0  = ((unsigned long)hardfault_args[0]);
  volatile unsigned long stacked_r1  = ((unsigned long)hardfault_args[1]);
  volatile unsigned long stacked_r2  = ((unsigned long)hardfault_args[2]);
  volatile unsigned long stacked_r3  = ((unsigned long)hardfault_args[3]);
  volatile unsigned long stacked_r12 = ((unsigned long)hardfault_args[4]);
  volatile unsigned long stacked_lr  = ((unsigned long)hardfault_args[5]);
  volatile unsigned long stacked_pc  = ((unsigned long)hardfault_args[6]);
  volatile unsigned long stacked_psr = ((unsigned long)hardfault_args[7]);
  volatile unsigned long _CFSR = (*((volatile unsigned long *)(0xE000ED28)));   //Configurable Fault Status Register
  volatile unsigned long _HFSR = (*((volatile unsigned long *)(0xE000ED2C)));   //Hard Fault Status Register
  volatile unsigned long _DFSR = (*((volatile unsigned long *)(0xE000ED30)));   //Debug Fault Status Register
  volatile unsigned long _AFSR = (*((volatile unsigned long *)(0xE000ED3C)));   //Auxiliary Fault Status Register
  volatile unsigned long _BFAR = (*((volatile unsigned long *)(0xE000ED38)));   //Read the Fault Address Registers. These may not contain valid values.
  volatile unsigned long _MMAR = (*((volatile unsigned long *)(0xE000ED34)));   //Check BFARVALID/MMARVALID to see if they are valid values
  __BKPT(0);                                                                    //Break into the debugger
}
// ---------------------------------------------------------------------------------------------------------------------
void HardFault_Handler(void) {
  __asm(  ".syntax unified\n"
          "MOVS   R0, #4  \n"
          "MOV    R1, LR  \n"
          "TST    R0, R1  \n"
          "BEQ    _MSP    \n"
          "MRS    R0, PSP \n"
          "B      HardFault_HandlerC \n"
          "_MSP: \n"
          "MRS    R0, MSP \n"
          "B      HardFault_HandlerC \n"
          ".syntax divided\n");
}
// ---------------------------------------------------------------------------------------------------------------------
const void *Vectors[] __attribute__((section(".vectors"), used)) = {            // Vector section placed at the beginning of flash by the linker script
  &STACK_END,                                                                   // 000 - Top of stack
  Sys_Init,                                                                     // 004 - Reset Handler
  Default_Handler,                                                              // 008 - NMI Handler
  HardFault_Handler,                                                            // 00C - HardFault Handler
  Default_Handler,                                                              // 010 - MemManage Fault Handler
  Default_Handler,                                                              // 014 - Bus Fault Handler
  Default_Handler,                                                              // 018 - Usage Fault Handler
  Default_Handler,                                                              // 01C - Reserved
  Default_Handler,                                                              // 020 - Reserved
  Default_Handler,                                                              // 024 - Reserved
  Default_Handler,                                                              // 028 - Reserved
  Default_Handler,                                                              // 02C - SVC Call Handler
  Default_Handler,                                                              // 030 - DebugMon Handler
  Default_Handler,                                                              // 034 - Reserved
  Default_Handler,                                                              // 038 - PendSV Handler
  Default_Handler,                                                              // 03C - SysTick Handler
  Default_Handler,                                                              // Window WatchDog
  Default_Handler,                                                              // PVD through EXTI Line detection
  Default_Handler,                                                              // Tamper and TimeStamps through the EXTI line
  Default_Handler,                                                              // RTC Wakeup through the EXTI line
  Default_Handler,                                                              // FLASH
  Default_Handler,                                                              // RCC
  Default_Handler,                                                              // EXTI Line0
  Default_Handler,                                                              // EXTI Line1
  Default_Handler,                                                              // EXTI Line2
  Default_Handler,                                                              // EXTI Line3
  Default_Handler,                                                              // EXTI Line4
  Default_Handler,                                                              // DMA1 Channel 1
  Default_Handler,                                                              // DMA1 Channel 2
  Default_Handler,                                                              // DMA1 Channel 3
  Default_Handler,                                                              // DMA1 Channel 4
  Default_Handler,                                                              // DMA1 Channel 5
  Default_Handler,                                                              // DMA1 Channel 6
  Default_Handler,                                                              // DMA1 Channel 7
  Default_Handler,                                                              // ADC1
  Default_Handler,                                                              // CAN1 TX
  Default_Handler,                                                              // CAN1 RX0
  Default_Handler,                                                              // CAN1 RX1
  Default_Handler,                                                              // CAN1 SCE
  Default_Handler,                                                              // External Line[9:5]s
  Default_Handler,                                                              // TIM15
  Default_Handler,                                                              // TIM16
  Default_Handler,                                                              // TIM17
  Default_Handler,                                                              // TIM18 and DAC2
  Default_Handler,                                                              // TIM2
  Default_Handler,                                                              // TIM3
  Default_Handler,                                                              // TIM4
  Default_Handler,                                                              // I2C1 Event
  Default_Handler,                                                              // I2C1 Error
  Default_Handler,                                                              // I2C2 Event
  Default_Handler,                                                              // I2C2 Error
  Default_Handler,                                                              // SPI1
  Default_Handler,                                                              // SPI2
  Default_Handler,                                                              // USART1
  Default_Handler,                                                              // USART2
  Default_Handler,                                                              // USART3
  Default_Handler,                                                              // External Line[15:10]s
  Default_Handler,                                                              // RTC_Alarm_IRQHandler
  Default_Handler,                                                              // CEC
  Default_Handler,                                                              // TIM12
  Default_Handler,                                                              // TIM13
  Default_Handler,                                                              // TIM14
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  Default_Handler,                                                              // TIM5
  Default_Handler,                                                              // SPI3
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  Default_Handler,                                                              // TIM6 and DAC1 Channel1 & channel2
  Default_Handler,                                                              // TIM7
  Default_Handler,                                                              // DMA2 Channel 1
  Default_Handler,                                                              // DMA2 Channel 2
  DMA2_CH3_ISR,                                                                 // DMA2 Channel 3
  Default_Handler,                                                              // DMA2 Channel 4
  Default_Handler,                                                              // DMA2 Channel 5
  Default_Handler,                                                              // SDADC1
  Default_Handler,                                                              // SDADC2
  Default_Handler,                                                              // SDADC3
  Default_Handler,                                                              // COMP
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  0,                                                                            // Reserved
  Default_Handler,                                                              // USB High Priority
  USB_LP_ISR,                                                                   // USB Low Priority
  Default_Handler,                                                              // USB Wakeup
  0,                                                                            // Resrved
  Default_Handler,                                                              // TIM19
  0,                                                                            // Reserved
  Default_Handler                                                               // FPU
};                                                                              //
// ---------------------------------------------------------------------------------------------------------------------