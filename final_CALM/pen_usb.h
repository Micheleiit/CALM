/*
* pen_usb.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Genstione del USB host 
*/

#ifndef PEN_USB_H
#define PEN_USB_H

/************************************************************** Define macros *********************************************************/
/**************************************************************************************************************************************/

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <MouseController.h> // Includi la libreria per il controllo del mouse

/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
struct USBStruct
{
  bool USBready;
};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initUSBStruct(USBStruct* usbst);
/**************************************************************************************************************************************/

#endif /* PEN_USB_H */