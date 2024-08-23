/*
* usb_manager.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Gestione protocollo USB 
*/

#ifndef USB_MANAGER_H
#define USB_MANAGER_H

/************************************************************* Define Macros **********************************************************/
/**************************************************************************************************************************************/

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include "Arduino.h"
#include <MouseController.h> // Includi la libreria per il controllo del mouse

/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
struct USBManager
{

  USBHost* usbHost; // puntatore all'oggetto USBHost. che è una classe utilizzata per gestire la comunicazione USB dal lato host. Il USBHost è responsabile dell'inizializzazione e del controllo della comunicazione USB, permettendo al sistema di interagire con dispositivi USB collegati.
  MouseController* mouse; // puntatore all'oggetto MouseController che è specificamente progettato per gestire l'interfaccia con un dispositivo mouse connesso tramite USB. Il MouseController si basa sul USBHost per eseguire il polling e gestire gli eventi del mouse
  
};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initUSBManager(USBManager* usbManager);
void updateUSB(USBManager* usbManager);

// Funzioni di callback per gestire gli eventi del mouse. Sarebbero le funzioni "weak" della libreria MouseController
void mouseMoved();
void mousePressed();
void mouseReleased();
/**************************************************************************************************************************************/

#endif /* USB_MANAGER_H */