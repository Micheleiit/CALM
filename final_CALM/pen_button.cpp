/*
* pen_button.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_button.h"

/*void initButtonStruct(ButtonStruct* buttonStruct){

  buttonStruct->leftButton = false;
  buttonStruct->middleButton = false;
  buttonStruct->rightButton = false;

  buttonStruct->lastRightPressTime = 0;
  buttonStruct->lastLeftPressTime = 0;
  buttonStruct->lastCenterPressTime = 0;

  buttonStruct->middleButtonClickCount = 0;
  
}*/

void initButtonStruct(ButtonStruct* buttonStruct){

  buttonStruct->pressState = false;
  buttonStruct->lastPressTime = 0;
  buttonStruct->buttonClickCount = 0;
  
}

// Definizione funzioni "weak" bottoni mouse

extern USBManager usbManager;
extern LedBlinkerStruct ledOk; // led giallo frontale
extern LedBlinkerStruct ledFault; // led rosso frontale
extern LedBlinkerStruct ledOnOff; // led bottone 
extern ButtonStruct rightButton;
extern ButtonStruct leftButton;
extern ButtonStruct middleButton;

void mousePressed(){

  if(usbManager.mouse.getButton(RIGHT_BUTTON) && !rightButton.pressState){
    rightButton.pressState = true;
    setLed(&ledOk, HIGH);
    rightButton.lastPressTime = millis();
  }

  if(usbManager.mouse.getButton(LEFT_BUTTON) && !leftButton.pressState){
    leftButton.pressState = true;
    setLed(&ledFault, HIGH);
    leftButton.lastPressTime = millis();

  }

  if(usbManager.mouse.getButton(MIDDLE_BUTTON) && !middleButton.pressState){
    middleButton.pressState = true;
    setLed(&ledOnOff, HIGH);
    middleButton.lastPressTime = millis();

  }

}