/*
* pen_zoom.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/
#include "pen_zoom.h"

void initZoomStruct(ZoomStruct* zoomStruct){

  zoomStruct->zoomAmount = 0;
  zoomStruct->zoomFactor = 0;

 zoomStruct->zoom_state = NORMAL_ZOOM;
 
}

// Definizioni funzioni "weak" bottoni mouse

extern USBManager usbManager;
extern ZoomStruct zoom;
extern LedBlinkerStruct ledOk; // led giallo frontale
extern LedBlinkerStruct ledFault; // led rosso frontale
extern LedBlinkerStruct ledOnOff; // led bottone 
extern LedBlinkerStruct ledGreen; // led verde sotto 
extern LedBlinkerStruct ledRed; // led rosso sotto 
extern ButtonStruct rightButton;
extern ButtonStruct leftButton;
extern ButtonStruct middleButton;

void mouseReleased(){

  if (!usbManager.mouse.getButton(RIGHT_BUTTON) && rightButton.pressState) {

    unsigned long elapsedTime = millis() - rightButton.lastPressTime;
    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido
      
      zoom.zoom_state = ZOOM_IN;

      setLed(&ledOk, LOW);
      //digitalWrite(LED_OK, LOW);
      
    }else{

      zoom.zoom_state = NORMAL_ZOOM;
      //delay(2000);
      enableLedBlink(&ledGreen, 500);
    }

    middleButton.buttonClickCount = 0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
    rightButton.pressState = false;
  }
}