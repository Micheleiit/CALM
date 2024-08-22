/*
* app.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "app.h"


void initAppStruct(AppStruct* app, SPIManager* app_spiManager, USBManager* app_usbManager, SPIStruct* app_penSpi, USBStruct* app_penUsb, 
                  PenMotionStruct* app_penMotion, ZoomStruct* app_zoom, ButtonStruct* app_button, LedBlinkerStruct* app_ledOk, LedBlinkerStruct* app_ledFault,
                  LedBlinkerStruct* app_ledOnOff, LedBlinkerStruct* app_ledRed, LedBlinkerStruct* app_ledGreen){
  
  app->spiManager = app_spiManager;
  app->usbManager = app_usbManager;

  app->penSpi = app_penSpi;
  app->penUsb = app_penUsb;

  app->penMotion = app_penMotion;
  app->zoom = app_zoom;
  app->button = app_button;
  
  app->ledOk = app_ledOk;
  app->ledFault = app_ledFault;
  app->ledOnOff = app_ledOnOff;
  app->ledRed = app_ledRed;
  app->ledGreen = app_ledGreen;
  
}