/* ================================ openZmeter ================================
 * File:     USB_Class.h
 * Author:   Eduardo Viciana (2018)
 * ---------------------------------------------------------------------------- */
#pragma once
#include "usb_lib.h"
//Descriptor defines ---------------------------------------------------------------------------------------------------
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
//Configuration defines ------------------------------------------------------------------------------------------------
#define USB_DATA_SIZE                             64
#define DEVICE_DESC_SIZE                          18
#define CONFIG_DESC_SIZE                          25
#define STRING_LANGID_SIZE                         4
#define STRING_VENDOR_SIZE                        10
#define STRING_PRODUCT_SIZE                       56
#define STRING_SERIAL_SIZE                        50
//Packet buffer area definition ----------------------------------------------------------------------------------------
#define BTABLE_ADDRESS                         0x000    /* Table start */
#define ENDP0_RXADDR                           0x040    /* EP0 - OUT */
#define ENDP0_TXADDR                           0x080    /* EP0 - IN */
#define ENDP1_BUF0ADDR                         0x0C0    /* EP1 - IN (Buff1) */
#define ENDP1_BUF1ADDR                         0x100    /* EP1 - IN (Buff2) */
//Class functions ------------------------------------------------------------------------------------------------------
#define CLASS_INTERFACE_SETSTATUS               0xFF
#define CLASS_INTERFACE_GETSTATUS               0xFE
//USB and CDC enums ----------------------------------------------------------------------------------------------------
typedef enum _RESUME_STATE { RESUME_EXTERNAL, RESUME_INTERNAL, RESUME_LATER, RESUME_WAIT, RESUME_START, RESUME_ON, RESUME_OFF, RESUME_ESOF } RESUME_STATE;
typedef enum _DEVICE_STATE { UNCONNECTED, ATTACHED, POWERED, SUSPENDED, ADDRESSED, CONFIGURED } DEVICE_STATE;
// Exported variables --------------------------------------------------------------------------------------------------
extern volatile uint8_t bDeviceState;
// Exported functions --------------------------------------------------------------------------------------------------
void USB_Init(void);

