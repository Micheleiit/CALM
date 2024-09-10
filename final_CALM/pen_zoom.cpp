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

  zoomStruct->zoomAmount = 1.0;
  zoomStruct->zoomFactor_old = 1.0;
  zoomStruct->zoomFactor_new = 1.0;
  
  zoomStruct->zoom_state_old= UNDEFINED;
  zoomStruct->zoom_state_new = NORMAL_ZOOM;
 
}

/************************************************************************* FUNZIONI WEAK DI MouseController **************************************************************/


void mouseReleased() {

  // Gestione del pulsante destro (ZOOM_IN)
  if (!usbManager.mouse.getButton(RIGHT_BUTTON) && rightButton.pressState) {

    unsigned long elapsedTime = millis() - rightButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido per ZOOM_IN

      setLed(&ledRed, HIGH); // LED ROSSO DI SOTTO
      setLed(&ledOnOff, LOW);
      setLed(&ledGreen, LOW); // LED ROSSO DI SOTTO

      rightButton.buttonClickCount ++; // Inizia il conteggio dei click

      switch(zoom.zoom_state_old){

        case NORMAL_ZOOM:

          zoom.zoom_state_new = ZOOM_IN;
          zoom.zoom_state_old = zoom.zoom_state_new;
          zoom.zoomFactor_new = ZOOM_IN_FACTOR + rightButton.buttonClickCount; //ZOOM_IN_FACTOR * rightButton.buttonClickCount; 
          zoom.zoomFactor_old = zoom.zoomFactor_new;
          zoom.zoomAmount = zoom.zoomFactor_new;
          
        break;

        case ZOOM_OUT:

          zoom.zoomFactor_old *= ((leftButton.buttonClickCount + 1) / leftButton.buttonClickCount); //pow(ZOOM_IN_FACTOR, rightButton.buttonClickCount); 
          zoom.zoomAmount = zoom.zoomFactor_old;
          leftButton.buttonClickCount--;

          if(leftButton.buttonClickCount != 0){
            zoom.zoom_state_old = ZOOM_OUT;
          }else{
            zoom.zoom_state_new = ZOOM_IN;
            zoom.zoom_state_old = zoom.zoom_state_new;
            rightButton.buttonClickCount = 1;
          }

        break;

        case ZOOM_IN:

          zoom.zoomFactor_new = constrain(ZOOM_IN_FACTOR + rightButton.buttonClickCount, 2, 4); // constrain(pow(ZOOM_IN_FACTOR, rightButton.buttonClickCount), 2, 4);
          zoom.zoomFactor_old = zoom.zoomFactor_new;
          zoom.zoomAmount = zoom.zoomFactor_new;
          
        break;
      }

    } else { // Click prolungato

      zoom.zoom_state_new = NORMAL_ZOOM;
      zoom.zoom_state_old = zoom.zoom_state_new;
      zoom.zoomFactor_new = 1.0;
      zoom.zoomFactor_old = zoom.zoomFactor_new ;
      zoom.zoomAmount = 1.0;
      rightButton.buttonClickCount = 0;
      setLed(&ledRed, LOW); 
      setLed(&ledOnOff, HIGH);
    
    }

    rightButton.pressState = false;
    
  }

  if (!usbManager.mouse.getButton(LEFT_BUTTON) && leftButton.pressState) {

    unsigned long elapsedTime = millis() - leftButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido per ZOOM_IN

      setLed(&ledGreen, HIGH); // LED VERDE DI SOTTO
      setLed(&ledOnOff, LOW);
      setLed(&ledRed, LOW); // LED ROSSO DI SOTTO

      leftButton.buttonClickCount ++; // Inizia il conteggio dei click

      switch(zoom.zoom_state_old) {

        case NORMAL_ZOOM:

          zoom.zoom_state_new = ZOOM_OUT;
          zoom.zoom_state_old = zoom.zoom_state_new;
          zoom.zoomFactor_new = 1.0/(ZOOM_OUT_FACTOR + leftButton.buttonClickCount); //ZOOM_OUT_FACTOR * rightButton.buttonClickCount; 
          zoom.zoomFactor_old = zoom.zoomFactor_new;
          zoom.zoomAmount = zoom.zoomFactor_new;
          
        break;

        case ZOOM_IN:

          zoom.zoomFactor_old -= 1 ; // /= pow(ZOOM_OUT_FACTOR, leftButton.buttonClickCount); 
          zoom.zoomAmount = zoom.zoomFactor_old;
          rightButton.buttonClickCount--;

          if(rightButton.buttonClickCount != 0){
            zoom.zoom_state_old = ZOOM_IN;
          }else{
            zoom.zoom_state_new = ZOOM_OUT;
            zoom.zoom_state_old = zoom.zoom_state_new;
            leftButton.buttonClickCount = 1;
          }

        break;
      
        case ZOOM_OUT:

          zoom.zoomFactor_new = 1.0/(constrain(ZOOM_OUT_FACTOR + leftButton.buttonClickCount, 2, 4)); //constrain(pow(ZOOM_OUT_FACTOR, rightButton.buttonClickCount), 2, 4);
          zoom.zoomFactor_old = zoom.zoomFactor_new;
          zoom.zoomAmount = zoom.zoomFactor_new;
        
        break;

      }

    } else { // Click prolungato

      zoom.zoom_state_new = NORMAL_ZOOM;
      zoom.zoom_state_old = zoom.zoom_state_new;
      zoom.zoomFactor_new = 1.0;
      zoom.zoomFactor_old = zoom.zoomFactor_new ;
      zoom.zoomAmount = 1.0;
      leftButton.buttonClickCount = 0;
      setLed(&ledGreen, LOW); 
      setLed(&ledOnOff, HIGH);
      

    }

    leftButton.pressState = false;
    
  }


  if (!usbManager.mouse.getButton(MIDDLE_BUTTON) && middleButton.pressState) {

    unsigned long elapsedTime = millis() - middleButton.lastPressTime;

    if (elapsedTime <= DEBOUNCE_DELAY) { // Click rapido

      middleButton.buttonClickCount++;

      penMotion.restart = true;

    }

    middleButton.pressState = false;


  }

}