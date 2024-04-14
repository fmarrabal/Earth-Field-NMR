/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
 * File Name          : usb_endp.c
 * Author             : MCD Application Team
 * Version            : V3.3.0
 * Date               : 21-March-2011
 * Description        : Endpoint routines
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/

#include "usb_lib.h"
#include "usb_mem.h"
#include "USB_CDC.h"
#include "usb_istr.h"
#include "usb_pwr.h"


uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];

uint8_t USB_Tx_State = 0;

//extern uint8_t USART_Rx_Buffer[];
//extern uint32_t USART_Rx_ptr_in;
//extern uint32_t USART_Rx_ptr_out;
//extern uint32_t USART_Rx_length;



/*	\brief Start Of Frame (SOF) callback
 */
void SOF_Callback(void) {
//  if (bDeviceState == CONFIGURED) {
    /* Check the data to be sent through IN pipe */
//    uint16_t USB_Tx_ptr;
//    uint16_t USB_Tx_length;

//    if(USB_Tx_State != 1) {
 //     if (USART_Rx_ptr_out == USART_RX_DATA_SIZE)     USART_Rx_ptr_out = 0;
 
   //   uint32_t in = USART_Rx_ptr_in;
//      if (USART_Rx_ptr_out == in) {
//        USB_Tx_State = 0;
//        return;
//      }

//      if (USART_Rx_ptr_out > in) /* rollback */ {
//        USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out + in;
//      } else {
//        USART_Rx_length = in - USART_Rx_ptr_out;
//      }

//      if (USART_Rx_length >= 8192)
//        USART_Rx_length = 8192;
//      else if (USART_Rx_length >= 4096)
//        USART_Rx_length = 4096;
//      else
//        return;

//      USB_Tx_ptr = USART_Rx_ptr_out;
//      USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

//      USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
//      USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;

//      USB_Tx_State = 1;

//      UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR,
//              USB_Tx_length);
//      SetEPTxCount(ENDP2, USB_Tx_length);
//      SetEPTxValid(ENDP2);
//    }
//  }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

