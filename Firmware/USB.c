// Modified by Eduardo Viciana (zRed)

// This file is the part of the Lightweight USB device Stack for STM32 microcontrollers
//
// Copyright ©2016 Dmitry Filimonchuk <dmitrystu[at]gmail[dot]com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//----------------------------------------------------------------------------------------------------------------------
#include "Main.h"
// USB control commands ------------------------------------------------------------------------------------------------
#define USB_SET_CONF   0x20                                                     //Set configuration command
#define USB_SET_DATA   0x21                                                     //Set LCD/SND data
//Defines --------------------------------------------------------------------------------------------------------------
#define USB_REQ_DIRECTION       (1 << 7)                                        //
#define USB_REQ_HOSTTODEV       (0 << 7)                                        //
#define USB_REQ_DEVTOHOST       (1 << 7)                                        //
#define USB_REQ_TYPE            (3 << 5)                                        //
#define USB_REQ_STANDARD        (0 << 5)                                        //
#define USB_REQ_CLASS           (1 << 5)                                        //
#define USB_REQ_VENDOR          (2 << 5)                                        //
#define USB_REQ_RECIPIENT       (3 << 0)                                        //
#define USB_REQ_DEVICE          (0 << 0)                                        //
#define USB_REQ_INTERFACE       (1 << 0)                                        //
#define USB_REQ_ENDPOINT        (2 << 0)                                        //
#define USB_REQ_OTHER           (3 << 0)                                        //
#define USB_EPDIR_IN                0x00                                        //
#define USB_EPDIR_OUT               0x80                                        //
#define USB_EPTYPE_CONTROL          0x00                                        //
#define USB_EPTYPE_ISOCHRONUS       0x01                                        //
#define USB_EPTYPE_BULK             0x02                                        //
#define USB_EPTYPE_DBLBUF           0x04                                        //
#define USB_EPTYPE_INTERRUPT        0x03                                        //
#define USB_DTYPE_DEVICE            0x01                                        //
#define USB_DTYPE_CONFIGURATION     0x02                                        //
#define USB_DTYPE_STRING            0x03                                        //
#define USB_DTYPE_INTERFACE         0x04                                        //
#define USB_DTYPE_ENDPOINT          0x05                                        //
#define USB_STD_GET_STATUS          0x00                                        //
#define USB_STD_CLEAR_FEATURE       0x01                                        //
#define USB_STD_SET_FEATURE         0x03                                        //
#define USB_STD_SET_ADDRESS         0x05                                        //
#define USB_STD_GET_DESCRIPTOR      0x06                                        //
#define USB_STD_SET_DESCRIPTOR      0x07                                        //
#define USB_STD_GET_CONFIG          0x08                                        //
#define USB_STD_SET_CONFIG          0x09                                        //
#define USB_STD_GET_INTERFACE       0x0A                                        //
#define USB_STD_SET_INTERFACE       0x0B                                        //
// Device descriptor ---------------------------------------------------------------------------------------------------
const uint8_t USB_Descriptor[] = {                                              //
  0x12,                                                                         //bLength
  USB_DTYPE_DEVICE,                                                             //bDescriptorType
  0x00, 0x02,                                                                   //bcdUSB = 2.00
  0xFF,                                                                         //bDeviceClass: vendor_defined
  0xFF,                                                                         //bDeviceSubClass
  0xFF,                                                                         //bDeviceProtocol
  0x40,                                                                         //bMaxPacketSize0
  0x47, 0x05,                                                                   //idVendor
  0x02, 0x10,                                                                   //idProduct
  0x00, 0x02,                                                                   //bcdDevice = 2.00
  1,                                                                            //Index of string descriptor describing manufacturer
  2,                                                                            //Index of string descriptor describing product
  3,                                                                            //Index of string descriptor describing the device's serial number
  0x01                                                                          //bNumConfigurations
};                                                                              //
const uint8_t USB_ConfigDescriptor[] = {                                        //Configuration descriptor
  0x09,                                                                         //bLength: Configuation Descriptor size
  USB_DTYPE_CONFIGURATION,                                                      //bDescriptorType: Configuration
  0x19, 0x00,                                                                   //wTotalLength:no of returned bytes
  0x01,                                                                         //bNumInterfaces: 1 interface
  0x01,                                                                         //bConfigurationValue: Configuration value
  0x00,                                                                         //iConfiguration: Index of string descriptor describing the configuration
  0xC0,                                                                         //bmAttributes: self powered
  0x00,                                                                         //MaxPower 0 mA
// Interface                                                                    //
  0x09,                                                                         //bLength: Interface Descriptor size
  USB_DTYPE_INTERFACE,                                                          //bDescriptorType: Interface
  0x00,                                                                         //bInterfaceNumber: Number of Interface
  0x00,                                                                         //bAlternateSetting: Alternate setting
  0x01,                                                                         //bNumEndpoints: 1 endpoints used
  0xFF,                                                                         //bInterfaceClass: Communication Interface Class
  0xFF,                                                                         //bInterfaceSubClass: Abstract Control Model
  0xFF,                                                                         //bInterfaceProtocol: No protocol
  0x00,                                                                         //iInterface:
// Endpoint                                                                     //
  0x07,                                                                         //bLength: Endpoint Descriptor size
  USB_DTYPE_ENDPOINT,                                                           //bDescriptorType: Endpoint
  0x81,                                                                         //bEndpointAddress: (IN1)
  0x02,                                                                         //bmAttributes: Bulk
  0x40, 0x00,                                                                   //wMaxPacketSize: 64
  0x00,                                                                         //bInterval: ignore for Bulk transfer
};                                                                              //
const uint8_t StringLangID[] = {                                                //String lang ID
  0x04,                                                                         //bLength
  USB_DTYPE_STRING,                                                             //bDescriptorType
  0x0A, 0x04                                                                    //LangID = 0x040A: Spanish
};                                                                              //
const uint8_t StringVendor[] = {                                                //Vendor name string
  0x0C,                                                                         //bLength
  USB_DTYPE_STRING,                                                             //bDescriptorType
  'C', 0, 'u', 0, 'r', 0, 'r', 0, 'o', 0                                        //
};                                                                              //
const uint8_t StringProduct[] = {                                               //Product name string
  0x20,                                                                         //bLength
  USB_DTYPE_STRING,                                                             //bDescriptorType
  'E', 0, 'a', 0, 'r', 0, 't', 0, 'h', 0, '-', 0, 'F', 0, 'i', 0,               //
  'e', 0, 'l', 0, 'd', 0, ' ', 0, 'N', 0, 'M', 0, 'R', 0                        //
};                                                                              //
// Access macros -------------------------------------------------------------------------------------------------------
#define EPR           ((volatile uint32_t *)USB_BASE)                           //
#define EPT           ((PMA_Table_t *)USB_PMAADDR)                              //
// Types ---------------------------------------------------------------------------------------------------------------
enum { USB_STATE_DEFAULT, USB_STATE_ADDRESSED, USB_STATE_CONFIGURED };          //USB device machine states
enum { USB_CONTROL_IDLE, USB_CONTROL_RXDATA, USB_CONTROL_TXDATA,                //USB device control endpoint machine state
       USB_CONTROL_TXPAYLOAD, USB_CONTROL_LASTDATA, USB_CONTROL_STATUSIN,       //
       USB_CONTROL_STATUSOUT };                                                 //
enum { USB_FAIL, USB_ACK, USB_NACK };                                           //Reporting status results
enum { USB_EVENT_TX, USB_EVENT_RX, USB_EVENT_SETUP };                           //Endpoint event received
typedef struct {                                                                //PMA record struct
  uint32_t     Addr;                                                            //
  uint32_t     Size;                                                            //
} PMA_Record_t;                                                                 //
typedef struct {                                                                //PMA table struct
  PMA_Record_t TX;                                                              //
  PMA_Record_t RX;                                                              //
} PMA_Table_t;                                                                  //
typedef struct {                                                                //Control request struct
  uint8_t      Type;                                                            //
  uint8_t      Request;                                                         //
  uint16_t     Value;                                                           //
  uint16_t     Index;                                                           //
  uint16_t     Length;                                                          //
  uint8_t      Data[64];                                                        //
} USB_ControlRequest_t;                                                         //
typedef struct {                                                                //Track device state
  uint8_t      Config:3;                                                        //Current device configuration number
  uint8_t      Control:3;                                                       //Current control transfer state
  uint8_t      Device:2;                                                        //Current state machine state
} State_t;                                                                      //
typedef void (*USB_Event_Callback_t)(const uint8_t event);                      //Used callbacks
typedef void (*USB_Request_Callback_t)();                                       //
// Variables -----------------------------------------------------------------------------------------------------------
static USB_ControlRequest_t   CtrReq;                                           //Active control transfer (if any)
static USB_Request_Callback_t OnComplete;                                       //Control transfer complete callback
static USB_Event_Callback_t   Endpoint[8];                                      //Callbacks of each endpoint
static uint8_t               *DataPtr;                                          //Pointer to current data for control request
static uint8_t                DataCount;                                        //Count remained data for control request
static State_t                State;                                            //Device state
// ---------------------------------------------------------------------------------------------------------------------
uint8_t USB_IsAddressed() {                                                     //Check if device is addressed
  volatile State_t *Tmp = &State;                                               //
  __NOP();
  return (Tmp->Device != USB_STATE_DEFAULT) ? 1 : 0;                            //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_HandleEP1_TX(uint8_t event) {                                          //Handle data sending
  if(event != USB_EVENT_TX) return;                                             //
  PMA_Record_t *Reg  = (EPR[1] & USB_EP_DTOG_RX) ? &EPT[1].RX : &EPT[1].TX;     //Select buffer to write
  uint32_t     *Dest = (uint32_t*)(USB_PMAADDR + 2 * Reg->Addr);                //
  BUFF_FillPMA(Dest);                                                           //Fill buffer
  Reg->Size = 64;                                                               //Transfer size
  EPR[1] = (EPR[1] & USB_EPREG_MASK) | USB_EP_DTOG_RX;                          //Enable buffer
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_IntToUnicode(uint32_t value, uint8_t *pbuf) {                          //Format serial number to buffer
  for(uint8_t i = 0; i < 8; i++) {                                              //
    *(pbuf++) = Int2Hex((value >> (4 * i))& 0x0F);                              //
    *(pbuf++) = 0;                                                              //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_StallEP(const uint8_t ep, const uint8_t stall) {                       //Stall endpoint
  uint8_t EP = ep & 0x07;                                                       //
  if(USB_EP_ISOCHRONOUS == (EPR[EP] & USB_EP_T_FIELD)) return;                  //ignore ISOCHRONOUS endpoints
  if(ep & USB_EPDIR_OUT) {                                                      //IN endpoint
    if(USB_EP_TX_DIS == (EPR[EP] & USB_EPTX_STAT)) return;                      //DISABLED endpoint can't be stalled or unstalled
    if(stall) {                                                                 //
      EPR[EP] = (EPR[EP] ^ USB_EP_TX_STALL) & (USB_EPREG_MASK | USB_EPTX_STAT); //
    } else {                                                                    //
      if((USB_EP_KIND|USB_EP_BULK) == (EPR[EP]&(USB_EP_T_FIELD|USB_EP_KIND))) { //doublebuffered endpoint
        EPR[EP] = (EPR[EP] ^ USB_EP_TX_VALID) &                                 //set endpoint to VALID and clear DTOG_TX & SWBUF_TX
            (USB_EPREG_MASK | USB_EPTX_STAT | USB_EP_DTOG_TX | USB_EP_DTOG_RX); //
      } else {                                                                  //
        EPR[EP] = (EPR[EP] ^ USB_EP_TX_NAK) &                                   //
            (USB_EPREG_MASK | USB_EPTX_STAT | USB_EP_DTOG_TX);                  //
      }                                                                         //
    }                                                                           //
  } else {                                                                      //
    if(USB_EP_RX_DIS == (EPR[EP] & USB_EPRX_STAT)) return;                      //DISABLED endpoint can't be stalled or unstalled
    if(stall) {                                                                 //
      EPR[EP] = (EPR[EP] ^ USB_EP_RX_STALL) & (USB_EPREG_MASK | USB_EPRX_STAT); //
    } else {                                                                    //
      if((USB_EP_KIND|USB_EP_BULK) == (EPR[EP]&(USB_EP_T_FIELD|USB_EP_KIND))) { //doublebuffered endpoint
        EPR[EP] = (EPR[EP] ^ (USB_EP_RX_VALID | USB_EP_DTOG_TX)) &              //set endpoint to VALID, clear DTOG_RX, set SWBUF_RX
            (USB_EPREG_MASK | USB_EPRX_STAT | USB_EP_DTOG_RX | USB_EP_DTOG_TX); //
      } else {                                                                  //
        EPR[EP] = (EPR[EP] ^ USB_EP_RX_VALID) &                                 //set endpoint to VALID and clear DTOG_RX
            (USB_EPREG_MASK | USB_EPRX_STAT | USB_EP_DTOG_RX);                  //
      }                                                                         //
    }                                                                           //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_RegisterEP(uint8_t ep, USB_Event_Callback_t callback) {                //Set endpoint event callback
  Endpoint[ep & 0x07] = callback;                                               //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_SetAddress() {                                                         //Set device address
  USB->DADDR = USB_DADDR_EF | CtrReq.Value;                                     //
  State.Device = (CtrReq.Value) ? USB_STATE_ADDRESSED : USB_STATE_DEFAULT;      //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_WritePMA(uint8_t *buf, uint16_t blen, PMA_Record_t *tx) {              //Write buffer to PMA
  uint32_t *Dest = (uint32_t*)(USB_PMAADDR + 2 * tx->Addr);                     //
  tx->Size = blen;                                                              //
  for(uint16_t n = 0, Word = 0; n < blen; n++, buf++) {                         //
    Word |= (*buf) << ((n & 0x01) ? 8 : 0);                                     //
    if((n & 0x01) || ((n + 1) == blen)) {                                       //
      *Dest++ = Word;                                                           //
      Word = 0;                                                                 //
    }                                                                           //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
uint16_t USB_ReadPMA(uint8_t *buf, uint16_t blen, PMA_Record_t *rx) {           //Fill buffer from PMA
  uint32_t *Src =  (uint32_t*)(USB_PMAADDR + 2 * rx->Addr);                     //
  blen = ((rx->Size & 0x03FF) < blen) ? (rx->Size & 0x03FF) : blen;             //
  for(uint16_t n = 0, Word = 0; n < blen; n++, buf++) {                         //
    if(n & 0x01) {                                                              //
      *buf = Word >> 8;                                                         //
    } else {                                                                    //
      Word = *Src++;                                                            //
      *buf = Word & 0xFF;                                                       //
    }                                                                           //
  }                                                                             //
  return blen;                                                                  //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
int16_t USB_WriteEP(uint8_t ep, uint8_t *buf, uint16_t blen) {                  //Send data on endpoint
  uint8_t EP = ep & 0x07;                                                       //
  switch(EPR[EP] & (USB_EPTX_STAT | USB_EP_T_FIELD | USB_EP_KIND)) {            //
    case (USB_EP_TX_NAK | USB_EP_BULK | USB_EP_KIND) :                          //doublebuffered bulk endpoint
      if(EPR[EP] & USB_EP_DTOG_RX) {                                            //
        USB_WritePMA(buf, blen, &(EPT[EP].RX));                                 //
      } else {                                                                  //
        USB_WritePMA(buf, blen, &(EPT[ep].TX));                                 //
      }                                                                         //
      EPR[EP] = (EPR[EP] & USB_EPREG_MASK) | USB_EP_DTOG_RX;                    //
      break;                                                                    //
    case (USB_EP_TX_VALID | USB_EP_ISOCHRONOUS) :                               //isochronous endpoint
      if((EPR[EP] & USB_EP_DTOG_TX) == 0) {                                     //
        USB_WritePMA(buf, blen, &(EPT[EP].RX));                                 //
      } else {                                                                  //
        USB_WritePMA(buf, blen, &(EPT[ep].TX));                                 //
      }                                                                         //
      break;                                                                    //
    case (USB_EP_TX_NAK | USB_EP_BULK) :                                        //regular endpoint
    case (USB_EP_TX_NAK | USB_EP_CONTROL) :                                     //
    case (USB_EP_TX_NAK | USB_EP_INTERRUPT) :                                   //
      USB_WritePMA(buf, blen, &(EPT[EP].TX));                                   //
      EPR[EP] = (EPR[EP] ^ USB_EP_TX_VALID) & (USB_EPREG_MASK | USB_EPTX_STAT); //
      break;                                                                    //
    default :                                                                   //invalid or not ready
      return -1;                                                                //
  }                                                                             //
  return blen;                                                                  //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
int16_t USB_ReadEP(uint8_t ep, uint8_t *buf, uint16_t blen) {                   //Read data from endpoint
  int32_t Read = 0;                                                             //
  uint8_t EP = ep & 0x07;                                                       //
  switch(EPR[EP] & (USB_EPRX_STAT | USB_EP_T_FIELD | USB_EP_KIND)) {            //
    case (USB_EP_RX_VALID | USB_EP_BULK | USB_EP_KIND) :                        //doublebuffered bulk endpoint
      switch(EPR[EP] & (USB_EP_DTOG_RX | USB_EP_DTOG_TX)) {                     //switching SWBUF if EP is NAKED
        case 0 :                                                                //
        case (USB_EP_DTOG_RX | USB_EP_DTOG_TX) :                                //
          EPR[EP] = (EPR[EP] & USB_EPREG_MASK) | USB_EP_DTOG_TX;                //
          break;                                                                //
        default:                                                                //
          break;                                                                //
      }                                                                         //
      if(EPR[EP] & USB_EP_DTOG_TX) {                                            //
        return USB_ReadPMA(buf, blen, &(EPT[EP].RX));                           //
      } else {                                                                  //
        return USB_ReadPMA(buf, blen, &(EPT[EP].TX));                           //
      }                                                                         //
    case (USB_EP_RX_VALID | USB_EP_ISOCHRONOUS) :                               //isochronous endpoint
      if(EPR[EP] & USB_EP_DTOG_RX) {                                            //
        return USB_ReadPMA(buf, blen, &(EPT[EP].RX));                           //
      } else {                                                                  //
        return USB_ReadPMA(buf, blen, &(EPT[EP].TX));                           //
      }                                                                         //
    case (USB_EP_RX_NAK | USB_EP_BULK) :                                        //regular endpoint
    case (USB_EP_RX_NAK | USB_EP_CONTROL) :                                     //
    case (USB_EP_RX_NAK | USB_EP_INTERRUPT) :                                   //
      Read = USB_ReadPMA(buf, blen, &(EPT[EP].RX));                             //
      EPR[EP] = (EPR[EP] ^ USB_EP_RX_VALID) & (USB_EPREG_MASK | USB_EPRX_STAT); //setting endpoint to VALID state
      return Read;                                                              //
    default:                                                                    //invalid or not ready
      return -1;                                                                //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
uint16_t USB_GetNextPMA(const uint16_t sz) {                                    //Get PMA address for new endpoints
  uint16_t Addr = 0x0200;                                                       //PMA block size
  for(uint8_t i = 0; i < 8; i++) {                                              //
    if((EPT[i].RX.Addr) && (EPT[i].RX.Addr < Addr)) Addr = EPT[i].RX.Addr;      //
    if((EPT[i].TX.Addr) && (EPT[i].TX.Addr < Addr)) Addr = EPT[i].TX.Addr;      //
  }                                                                             //
  return Addr - sz;                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_ConfigEP(uint8_t ep, uint8_t type, uint16_t size) {                    //Configure endpoint
  uint8_t EP = (ep & 0x07);                                                     //
  if(type == USB_EPTYPE_CONTROL) {                                              //Set type and number
    EPR[EP] = USB_EP_CONTROL | EP;                                              //
  } else if(type == USB_EPTYPE_BULK) {                                          //
    EPR[EP] = USB_EP_BULK | EP;                                                 //
  } else if(type == (USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF)) {                    //
    EPR[EP] = USB_EP_BULK | USB_EP_KIND | EP;                                   //
  } else {                                                                      //
    EPR[EP] = USB_EP_INTERRUPT | EP;                                            //
  }                                                                             //
  size = (~0x01U) & (size + 1);                                                 //Force 2-byte aligned size
  if(((ep & USB_EPDIR_OUT) != 0) || (type == USB_EPTYPE_CONTROL)) {             //Set IN endpoints address
    EPT[EP].TX.Addr = USB_GetNextPMA(size);                                     //
    EPT[EP].TX.Size  = 0;                                                       //
    if(type == (USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF)) {                         //
      EPT[EP].RX.Addr = USB_GetNextPMA(size);                                   //
      EPT[EP].RX.Size  = 0;                                                     //
      EPR[EP] = (EPR[EP] ^ USB_EP_TX_VALID) &                                   //
           (USB_EPREG_MASK | USB_EPTX_STAT | USB_EP_DTOG_TX | USB_EP_DTOG_RX);  //
    } else {                                                                    //
      EPR[EP] = (EPR[EP] ^ USB_EP_TX_NAK) &                                     //
           (USB_EPREG_MASK | USB_EPTX_STAT | USB_EP_DTOG_TX);                   //
    }                                                                           //
  }                                                                             //
  if((ep & USB_EPDIR_OUT) == 0) {                                               //Set OUT endpoints address
    if(size > 62) {                                                             //
      size = (~0x1FU) & (size + 0x1FU);                                         //On larger than 62bytes force 32-byte aligned address
      EPT[EP].RX.Size = 0x8000U - 0x400U + (size << 5);                         //
    } else {                                                                    //
      EPT[EP].RX.Size = size << 9;                                              //
    }                                                                           //
    EPT[EP].RX.Addr = USB_GetNextPMA(size);                                     //
    if((type == (USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF))) {                       //
      EPT[EP].TX.Addr = USB_GetNextPMA(size);                                   //
      EPT[EP].TX.Size  = EPT[EP].RX.Size;                                       //
      EPR[EP] = (EPR[EP] ^ (USB_EP_RX_VALID | USB_EP_DTOG_TX)) &                //
           (USB_EPREG_MASK | USB_EPRX_STAT | USB_EP_DTOG_RX | USB_EP_DTOG_TX);  //
    } else {                                                                    //
      EPR[EP] = (EPR[EP] ^ USB_EP_RX_VALID) &                                   //
           (USB_EPREG_MASK | USB_EPRX_STAT | USB_EP_DTOG_RX);                   //
    }                                                                           //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_DeconfigEP(const uint8_t ep) {                                         //Deconfigure endpoint
  uint8_t EP = ep & 0x07;                                                       //
  EPR[EP] &= ~USB_EPREG_MASK;                                                   //
  EPT[EP].RX.Addr = 0;                                                          //
  EPT[EP].RX.Size = 0;                                                          //
  EPT[EP].TX.Addr = 0;                                                          //
  EPT[EP].TX.Size = 0;                                                          //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
uint8_t USB_HandleEP0_Process() {                                               //Handle EP0 requests
  uint8_t Type = CtrReq.Type & (USB_REQ_TYPE | USB_REQ_RECIPIENT);              //
  if(Type == (USB_REQ_STANDARD | USB_REQ_DEVICE)) {                             //
    if(CtrReq.Request == USB_STD_GET_STATUS) {                                  //Standard-Device-GetStatus request
      CtrReq.Data[0] = 0;                                                       //
      CtrReq.Data[1] = 0;                                                       //
      return USB_ACK;                                                           //
    } else if(CtrReq.Request == USB_STD_GET_CONFIG) {                           //Standard-Device-GetConfig request
      CtrReq.Data[0] = State.Config;                                            //
      return USB_ACK;                                                           //
    } else if(CtrReq.Request == USB_STD_SET_ADDRESS) {                          //Standard-Device-SetAddress request
      OnComplete = USB_SetAddress;                                              //
      return USB_ACK;                                                           //
    } else if(CtrReq.Request == USB_STD_GET_DESCRIPTOR) {                       //Standard-Device-GetDescriptor request
      if((CtrReq.Value >> 8) == USB_DTYPE_STRING) {                             //
        if((CtrReq.Value & 0xFF) == 0x00) {                                     //
          DataPtr = (uint8_t*) StringLangID;                                    //
          DataCount = StringLangID[0];                                          //
          return USB_ACK;                                                       //
        } else if ((CtrReq.Value & 0xFF) == 0x01) {                             //
          DataPtr = (uint8_t*) StringVendor;                                    //
          DataCount = StringVendor[0];                                          //
          return USB_ACK;                                                       //
        } else if ((CtrReq.Value & 0xFF) == 0x02) {                             //
          DataPtr = (uint8_t*) StringProduct;                                   //
          DataCount = StringProduct[0];                                         //
          return USB_ACK;                                                       //
        } else if ((CtrReq.Value & 0xFF) == 0x03) {                             //
          uint8_t *dsc = CtrReq.Data;                                           //
          dsc[0] = 50;                                                          //
          dsc[1] = USB_DTYPE_STRING;                                            //
          USB_IntToUnicode(((uint32_t*)UID_BASE)[0], &dsc[2]);                  //
          USB_IntToUnicode(((uint32_t*)UID_BASE)[1], &dsc[18]);                 //
          USB_IntToUnicode(((uint32_t*)UID_BASE)[2], &dsc[34]);                 //
          DataCount = 50;                                                       //
          return USB_ACK;                                                       //
        }                                                                       //
      } else if((CtrReq.Value >> 8) == USB_DTYPE_DEVICE) {                      //
        DataPtr = (uint8_t*)USB_Descriptor;                                     //
        DataCount = USB_Descriptor[0];                                          //
        return USB_ACK;                                                         //
      } else if ((CtrReq.Value >> 8) == USB_DTYPE_CONFIGURATION) {              //
        DataPtr = (uint8_t*)USB_ConfigDescriptor;                               //
        DataCount = sizeof(USB_ConfigDescriptor);                               //
        return USB_ACK;                                                         //
      }                                                                         //
    } else if(CtrReq.Request == USB_STD_SET_CONFIG) {                           //Standard-Device-SetConfig request
      if(CtrReq.Value != 1) {                                                   //
        for(uint8_t i = 1; i < 8; i++) USB_DeconfigEP(i);                       //Deconfigure device
        for(uint8_t i = 1; i < 8; i++) USB_RegisterEP(i, NULL);                 //
        State.Device = USB_STATE_ADDRESSED;                                     //
        State.Config = CtrReq.Value & 0x07;                                     //
        return USB_ACK;                                                         //
      } else if(CtrReq.Value == 1) {                                            //
        USB_ConfigEP(0x81, USB_EPTYPE_BULK | USB_EPTYPE_DBLBUF, 64);            //Configure device
        USB_RegisterEP(0x81, USB_HandleEP1_TX);                                 //
        State.Device = USB_STATE_CONFIGURED;                                    //
        State.Config = CtrReq.Value & 0x07;                                     //
        return USB_ACK;                                                         //
      }                                                                         //
    }                                                                           //
  } else if(Type == (USB_REQ_STANDARD | USB_REQ_INTERFACE)) {                   //
    if(CtrReq.Request == USB_STD_GET_STATUS) {                                  //Standard-Interface-GetStatus
      CtrReq.Data[0] = 0;                                                       //
      CtrReq.Data[1] = 0;                                                       //
      return USB_ACK;                                                           //
    }                                                                           //
  } else if(Type == (USB_REQ_STANDARD | USB_REQ_ENDPOINT)) {                    //
    if(CtrReq.Request == USB_STD_SET_FEATURE) {                                 //Standard-Endpoint-SetFeature
      USB_StallEP(CtrReq.Index, 1);                                             //
      return USB_ACK;                                                           //
    } else if(CtrReq.Request == USB_STD_CLEAR_FEATURE) {                        //Standard-Endpoint-ClearFeature
      USB_StallEP(CtrReq.Index, 0);                                             //
      return USB_ACK;                                                           //
    } else if(CtrReq.Request == USB_STD_GET_STATUS) {                           //Standard-Endpoint-GetStatus
      CtrReq.Data[0] = 0;                                                       //
      if(CtrReq.Index & USB_EPDIR_OUT) {                                        //
        if(USB_EPTX_STAT & EPR[CtrReq.Index & 0x07]) CtrReq.Data[0] = 1;        //
      } else {                                                                  //
        if(USB_EPRX_STAT & EPR[CtrReq.Index & 0x07]) CtrReq.Data[0] = 1;        //
      }                                                                         //
      CtrReq.Data[1] = 0;                                                       //
      return USB_ACK;                                                           //
    }                                                                           //
  } else if(Type == (USB_REQ_VENDOR | USB_REQ_DEVICE)) {                        //openZmeter control request
    if(CtrReq.Request == 0x20) {                                                //Implemented commands
      SetTimes(CtrReq.Index, CtrReq.Value);                                     //Set time for pulse/recovery
    } else if(CtrReq.Request == 0x21) {                                         //Set pre/post delay for relay
      SetDelays(CtrReq.Index, CtrReq.Value);                                    //
    } else if(CtrReq.Request == 0x22) {                                         //Set number of samples per scan
      SetSamples(((uint32_t)CtrReq.Index << 16) | CtrReq.Value);           //
    } else if(CtrReq.Request == 0x23) {                                         //Start capture
      Capture(CtrReq.Value);                                                    //
    } else {                                                                    //
      return USB_FAIL;                                                          //If command not implemented
    }                                                                           //
    return USB_ACK;                                                             //
  }                                                                             //
  return USB_FAIL;                                                              //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_StallEP0() {                                                           //Stall EP0
  USB_StallEP(0x00, 1);                                                         //
  USB_StallEP(0x80, 1);                                                         //
  State.Control = USB_CONTROL_IDLE;                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_HandleEP0_TX() {                                                       //Handle TX control transfers
  int16_t Write = 0;                                                            //
  switch(State.Control) {                                                       //
    case USB_CONTROL_TXPAYLOAD:                                                 //
    case USB_CONTROL_TXDATA:                                                    //
      Write = (DataCount < sizeof(CtrReq.Data)) ? DataCount:sizeof(CtrReq.Data);//
      USB_WriteEP(0, DataPtr, Write);                                           //
      DataPtr += Write;                                                         //
      DataCount -= Write;                                                       //
      if(DataCount != 0) return;                                                //if all data is not sent
      if((State.Control == USB_CONTROL_TXDATA)||(sizeof(CtrReq.Data) != Write)){//If no ZLP needed
        State.Control = USB_CONTROL_LASTDATA;                                   //
      }                                                                         //
      return;                                                                   //
    case USB_CONTROL_LASTDATA:                                                  //On ZLP needed do nothing
      State.Control = USB_CONTROL_STATUSOUT;                                    //
      return;                                                                   //
    case USB_CONTROL_STATUSIN:                                                  //Terminate transaction
      State.Control = USB_CONTROL_IDLE;                                         //
      if(OnComplete) OnComplete();                                              //Call complete callback
      return;                                                                   //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_HandleEP0_RX() {                                                       //Handle RX control transfers
  uint16_t Read = 0;                                                            //
  switch(State.Control) {                                                       //
    case USB_CONTROL_IDLE :                                                     //read SETUP packet
      if(USB_ReadEP(0, (uint8_t*)&CtrReq, sizeof(CtrReq.Data)) != 0x08) {       //Stall if incorrect packet length
        USB_StallEP0();                                                         //
        return;                                                                 //
      }                                                                         //
      DataPtr = CtrReq.Data;                                                    //Init reception pointer
      DataCount = CtrReq.Length;                                                //
      if((CtrReq.Type & USB_REQ_DEVTOHOST) || (CtrReq.Length == 0)) break;      //if no payload data
      if(CtrReq.Length > sizeof(CtrReq.Data)) return USB_StallEP0(0);           //check available memory for DATA OUT
      State.Control = USB_CONTROL_RXDATA;                                       //continue DATA OUT stage
      return;                                                                   //
    case USB_CONTROL_RXDATA :                                                   //receive DATA OUT packet(s)
      Read = USB_ReadEP(0, DataPtr, DataCount);                                 //Get received data
      if(Read > DataCount) {                                                    //If more data than expected
        return USB_StallEP0();                                                  //
      } else if(DataCount != Read) {                                            //if all data payload was not received yet
        DataCount -= Read;                                                      //
        DataPtr += Read;                                                        //
        return;                                                                 //
      }                                                                         //
      break;                                                                    //
    case USB_CONTROL_STATUSOUT :                                                //reading STATUS OUT data to buffer
      USB_ReadEP(0, 0, 0);                                                      //
      State.Control = USB_CONTROL_IDLE;                                         //
      if(OnComplete) OnComplete();                                              //Call complete callback
      return;                                                                   //
    default  :                                                                  //unexpected RX packet
      return USB_StallEP0();                                                    //
  }                                                                             //
  DataPtr = CtrReq.Data;                                                        //Handle received data
  DataCount = sizeof(CtrReq.Data);                                              //
  switch(USB_HandleEP0_Process()) {                                             //
    case USB_ACK :                                                              //On success
      if(CtrReq.Type & USB_REQ_DEVTOHOST) {                                     //
        if(DataCount >= CtrReq.Length) {                                        //return data from function
          DataCount = CtrReq.Length;                                            //
          State.Control = USB_CONTROL_TXDATA;                                   //
        } else {                                                                //
          State.Control = USB_CONTROL_TXPAYLOAD;                                //
        }                                                                       //
        USB_HandleEP0_TX();                                                     //
      } else {                                                                  //
        USB_WriteEP(0x80, 0, 0);                                                //confirming by ZLP in STATUS_IN stage
        State.Control = USB_CONTROL_STATUSIN;                                   //
      }                                                                         //
      break;                                                                    //
    case USB_NACK :                                                             //
      State.Control = USB_CONTROL_STATUSIN;                                     //
      break;                                                                    //
    default :                                                                   //
      return USB_StallEP0();                                                    //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_HandleEP0(const uint8_t evt) {                                         //Handle EP0 CONTROL transfers
  if(evt == USB_EVENT_TX) {                                                     //
    USB_HandleEP0_TX();                                                         //
  } else {                                                                      //
    if(evt == USB_EVENT_SETUP) {                                                //
      State.Control = USB_CONTROL_IDLE;                                         //
      OnComplete = NULL;                                                        //
    }                                                                           //
    USB_HandleEP0_RX();                                                         //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_LP_ISR() {                                                             //USB interrupt handler
  if(USB->ISTR & USB_ISTR_CTR) {                                                //Manage correct transfer complete
    uint8_t EP = USB->ISTR & 0x07;                                              //
    if(EPR[EP] & USB_EP_CTR_TX) {                                               //
      EPR[EP] &= (USB_EPREG_MASK ^ USB_EP_CTR_TX);                              //
      if(Endpoint[EP]) Endpoint[EP](USB_EVENT_TX);                              //
    } else {                                                                    //
      EPR[EP] &= (USB_EPREG_MASK ^ USB_EP_CTR_RX);                              //
      if(Endpoint[EP]) {                                                        //
        Endpoint[EP]((EPR[EP] & USB_EP_SETUP) ? USB_EVENT_SETUP : USB_EVENT_RX);//
      }                                                                         //
    }                                                                           //
  } else if(USB->ISTR & USB_ISTR_RESET) {                                       //Manage bus reset
    USB->ISTR &= ~USB_ISTR_RESET;                                               //
    USB->BTABLE = 0;                                                            //
    for(uint8_t i = 0; i < 8; i++) USB_DeconfigEP(i);                           //
    State.Device = USB_STATE_DEFAULT;                                           //
    State.Control = USB_CONTROL_IDLE;                                           //
    State.Config = 0;                                                           //
    USB_ConfigEP(0, USB_EPTYPE_CONTROL, sizeof(CtrReq.Data));                   //
    Endpoint[0] = USB_HandleEP0;                                                //
    USB->DADDR = USB_DADDR_EF;                                                  //
  } else if(USB->ISTR & USB_ISTR_WKUP) {                                        //Manage bus wake up
    USB->CNTR &= ~USB_CNTR_FSUSP;                                               //
    USB->ISTR &= ~USB_ISTR_WKUP;                                                //
  } else if (USB->ISTR & USB_ISTR_SUSP) {                                       //Manage bus suspend
    USB->CNTR |= USB_CNTR_FSUSP;                                                //
    USB->ISTR &= ~USB_ISTR_SUSP;                                                //
  } else if (USB->ISTR & USB_ISTR_ERR) {                                        //Manage bus error
    USB->ISTR &= ~USB_ISTR_ERR;                                                 //
  }                                                                             //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------
void USB_Init() {                                                               //Init USB port
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_USBEN;                        //Enable clock to USB peripheral
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;                       //
  GPIOA->MODER |= (0b10<<GPIO_MODER_MODER11_Pos)|(0b10<<GPIO_MODER_MODER12_Pos);//Configure ports
  GPIOB->MODER |= GPIO_MODER_MODER5_0;                                          //Pullup resistor
  GPIOA->OSPEEDR |= (0b11 << GPIO_OSPEEDER_OSPEEDR11_Pos) |                     //High speed mode
                    (0b11 << GPIO_OSPEEDER_OSPEEDR12_Pos);                      //
  GPIOA->AFR[1] |= (14 << GPIO_AFRH_AFRH3_Pos)|(14 << GPIO_AFRH_AFRH4_Pos);     //Set port PA11 and PA12 to alternate function USB
  USB->CNTR = USB_CNTR_CTRM | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_ERRM | //Enabled interrupts
              USB_CNTR_RESETM;                                                  //
  GPIOB->BSRR = GPIO_BSRR_BS_5;                                                 //Enable resistor
  DataPtr = (uint8_t*)&CtrReq;                                                  //Configure control request buffer
  NVIC_SetPriority(USB_LP_IRQn, 0);                                             //Enable interrupts
  NVIC_EnableIRQ(USB_LP_IRQn);                                                  //
}                                                                               //
// ---------------------------------------------------------------------------------------------------------------------