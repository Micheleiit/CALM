/*
* app.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "app.h"

void initAppStruct(AppStruct* app, PenMotionStruct* app_penMotion, SPIStruct* app_penSpi, USBStruct* app_usb, ZoomStruct* app_zoom, ButtonStruct* app_button, SPIManager* app_spiManager){
  
  app->penMotion = app_penMotion;
  app->penSpi = app_penSpi;
  app->usb = app_usb;
  app->zoom = app_zoom;
  app->button = app_button;

  app->spiManager = app_spiManager;

}