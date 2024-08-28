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

extern float zoomScale;

void initZoomStruct(ZoomStruct* zoomStruct){

  zoomStruct->zoomAmount = 0;
  zoomStruct->zoomFactor = 1.0;

  zoomStruct->zoom_state = NORMAL_ZOOM;
 
}

void updateZoomState(ZoomState state){
  switch (state) {

    case ZOOM_IN:

      setLed(&ledOk, HIGH);    // Accendi il LED giallo per ZOOM_IN
      setLed(&ledOnOff, LOW);  // Spegni il LED verde
      setLed(&ledFault, LOW);    // Spegni il LED rosso

    break;

    case ZOOM_OUT:

      setLed(&ledFault, HIGH);   // Accendi il LED rosso per ZOOM_OUT
      setLed(&ledOnOff, LOW);  // Spegni il LED verde
      setLed(&ledOk, LOW);     // Spegni il LED giallo

    break;

    case NORMAL_ZOOM:

      setLed(&ledOnOff, HIGH); // Accendi il LED verde per NORMAL_ZOOM
      setLed(&ledOk, LOW);     // Spegni il LED giallo
      setLed(&ledFault, LOW);    // Spegni il LED rosso

    break;
  }
}

/************************************************************************* FUNZIONI WEAK DI MouseController **************************************************************/


void mouseReleased(){

  if (!usbManager.mouse.getButton(RIGHT_BUTTON) && rightButton.pressState) {

    unsigned long elapsedTime = millis() - rightButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido
      
      zoom.zoom_state = ZOOM_IN;
      zoom.zoomFactor = ZOOM_IN_FACTOR; // fisso il fattore di scala a 2X

      middleButton.buttonClickCount = 1; //0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
      zoomScale *= ZOOM_IN_FACTOR * middleButton.buttonClickCount; 
      zoom.zoomFactor = zoomScale; //*= middleButton.buttonClickCount;

    }else{

      zoom.zoom_state = NORMAL_ZOOM;
      
    }

    rightButton.pressState = false;

    updateZoomState(zoom.zoom_state);

  }

  if (!usbManager.mouse.getButton(LEFT_BUTTON) && leftButton.pressState) {

    unsigned long elapsedTime = millis() - leftButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido
      
      zoom.zoom_state = ZOOM_OUT;
      zoom.zoomFactor = ZOOM_OUT_FACTOR; // fisso il fattore di scala a 2X

      middleButton.buttonClickCount = 1; //0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
      zoomScale *= 1.0/ZOOM_OUT_FACTOR * middleButton.buttonClickCount; 
      zoom.zoomFactor = zoomScale; //*= middleButton.buttonClickCount;
      
    }else{

      zoom.zoom_state = NORMAL_ZOOM;
    
    }

    leftButton.pressState = false;

    updateZoomState(zoom.zoom_state);

  }

  if (!usbManager.mouse.getButton(MIDDLE_BUTTON) && middleButton.pressState) {

    unsigned long elapsedTime = millis() - middleButton.lastPressTime;
    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido

      switch (zoom.zoom_state){

        case ZOOM_IN:

          middleButton.buttonClickCount++;
          zoom.zoomFactor *= middleButton.buttonClickCount; // Moltiplico il fattore di scala fissato per una costante (n° click middleButton)

        break;

        case ZOOM_OUT:
        
          middleButton.buttonClickCount++;
          zoom.zoomFactor *= middleButton.buttonClickCount; // Moltiplico il fattore di scala fissato per una costante (n° click middleButton)

        break;

        default:

          zoom.zoomFactor = 1.0;

        break;

      }

    }else{

      // blink dei led sotto
      doubleLedBlink(&ledRed, &ledGreen, BLINK_PERIOD, 2);
  
      penMotion.restart = true; // azzeramento del pennino
      
    }

    middleButton.pressState = false;

  }

}