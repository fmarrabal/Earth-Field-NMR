#include "Main.h"
//----------------------------------------------------------------------------------------------------------------------
#define BUFFER_SIZE         8192                                                //Buffer size
//----------------------------------------------------------------------------------------------------------------------
static uint16_t     Head;                                                       //Buffer head
static uint16_t     Tail;                                                       //Buffer tail
static uint32_t     Free;                                                       //Free buffer space
static uint32_t     Used;                                                       //Used buffer space
static uint16_t     Buffer[BUFFER_SIZE];                                        //Data buffer (as big as posible)
static uint16_t     Scan;                                                       //Current scan
// ---------------------------------------------------------------------------------------------------------------------
void BUFF_PushSample(const uint16_t value) {                                    //Add sample to buffer
  if(Free == 0) return;                                                         //If no free space return
  Buffer[Head] = value;                                                         //Otherwise, append to buffer
  Free--;                                                                       //
  Used++;                                                                       //
  Head++;                                                                       //
  if(Head == BUFFER_SIZE) Head = 0;                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void BUFF_FillPMA(uint32_t *buffer) {                                           //Build a transfer buffer and move to PMA
  uint8_t Count = 0;                                                            //Init sended samples
  uint8_t Filled = 0;                                                           //Filled values
  while(Filled < 31) {                                                          //While space in buffer
    if(Used == 0) break;                                                        //If no more samples break
    *buffer++ = Buffer[Tail];                                                   //Place samples to fill block
    Used--;                                                                     //Update counters
    Free++;                                                                     //
    Tail++;                                                                     //
    if(Tail == BUFFER_SIZE) Tail = 0;                                           //
    Count++;                                                                    //
    Filled++;                                                                   //
  }                                                                             //
  while(Filled < 31) {                                                          //Fill rest of buffer
    *buffer++ = 0;                                                              //
    Filled++;                                                                   //
  }                                                                             //
  *buffer++ = ((Scan & 0x07FF) << 5) | (Count & 0x1F);                          //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void BUFF_IncScan() {
  Scan++;
}
// ---------------------------------------------------------------------------------------------------------------------
void BUFF_Init() {                                                              //Initialize buffer
  Scan = 0;                                                                     //
  Head = 0;                                                                     //Buffer head
  Tail = 0;                                                                     //Buffer tail
  Free = BUFFER_SIZE;                                                           //Free buffer space
  Used = 0;                                                                     //Used buffer space
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------