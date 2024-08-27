/*
* pen_zoom.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/
#include "pen_zoom.h"

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

extern PenMotionStruct penMotion;

void initZoomStruct(ZoomStruct* zoomStruct){

  zoomStruct->zoomAmount = 0;
  zoomStruct->zoomFactor = 0;

  zoomStruct->zoom_state = NORMAL_ZOOM;
 
}

/************************************************************************* FUNZIONI WEAK DI MouseController **************************************************************/


void mouseReleased(){

  if (!usbManager.mouse.getButton(RIGHT_BUTTON) && rightButton.pressState) {

    unsigned long elapsedTime = millis() - rightButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido
      
      zoom.zoom_state = ZOOM_IN;
      digitalWrite(LED_OK, HIGH);

    }else{

      zoom.zoom_state = NORMAL_ZOOM;
      digitalWrite(LED_OK, LOW);
      
      
    }

    middleButton.buttonClickCount = 0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
    rightButton.pressState = false;
  }

  if (!usbManager.mouse.getButton(LEFT_BUTTON) && leftButton.pressState) {

    unsigned long elapsedTime = millis() - leftButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido
      
      zoom.zoom_state = ZOOM_OUT;
      digitalWrite(LED_FAULT, HIGH);
      
    }else{

      zoom.zoom_state = NORMAL_ZOOM;
      digitalWrite(LED_FAULT, LOW);
    
    }

    middleButton.buttonClickCount = 0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
    leftButton.pressState = false;
  }

  if (!usbManager.mouse.getButton(MIDDLE_BUTTON) && middleButton.pressState) {

    unsigned long elapsedTime = millis() - middleButton.lastPressTime;
    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido

      switch (zoom.zoom_state){

        case ZOOM_IN:
        zoom.zoomFactor = ZOOM_IN_FACTOR; // fisso il fattore di scala a 2X
        break;

        case ZOOM_OUT:
        zoom.zoomFactor = ZOOM_OUT_FACTOR; // fisso il fattore di scala a 2X
        break;

      }
  
      // Incrementa il contatore dei click del middleButton
      //digitalWrite(BUZZER, HIGH);

      middleButton.buttonClickCount++;
      zoom.zoomFactor *= middleButton.buttonClickCount; // Moltiplico il fattore di scala fissato per una costante (n° click middleButton)

    }else{

      // blink dei led sotto
      doubleLedBlink(&ledRed, &ledGreen, BLINK_PERIOD, 3);
  
      penMotion.restart = true; // azzeramento del pennino
      
    }

    middleButton.pressState = false;

  }

}