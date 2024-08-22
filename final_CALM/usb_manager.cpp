/*
* usb_manager.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "usb_manager.h"

// Inizializzo l'USBManager con un USBHost e MouseController
void initUSBManager(USBManager* usbManager){
  usbManager->usbHost = new USBHost(); // creo un nuovo oggetto USBHost
  usbManager->mouse = new MouseController(*(usbManager->usbHost)); // Associa il MouseController al  USBHost
  //usbManager->usbHost->begin(); // inizializza l'host USB
}

// Funzione per aggiornare l'interfaccia USB (esegue il polling del mouse)
void updateUSB(USBManager* usbManager){
  usbManager->usbHost->Task(); // Esegui il task di gestione USB
}
