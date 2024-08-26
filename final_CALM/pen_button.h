/*
* pen_button.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Gestione bottoni pennino 
*/

#ifndef PEN_BUTTON_H
#define PEN_BUTTON_H

/************************************************************** Define macros *********************************************************/
#define DEBOUNCE_DELAY 200 // Tempo massimo (in ms) tra pressione e rilascio per considerare un "click" rapido

/**************************************************************************************************************************************/

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>

#include "led_blinker.h"
#include "usb_manager.h"
#include "MouseController.h"
/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
/*struct ButtonStruct
{
  bool leftButton;
  bool middleButton;
  bool rightButton;

  unsigned long lastRightPressTime;
  unsigned long lastLeftPressTime;
  unsigned long lastCenterPressTime;

  //unsigned long debounceDelay; // Tempo massimo (in ms) tra pressione e rilascio per considerare un "click" rapido

  unsigned int middleButtonClickCount; // variabile per contare i click del middleButton
};*/

struct ButtonStruct
{
  bool pressState;
  unsigned long lastPressTime;
  unsigned int buttonClickCount; // variabile per contare i click del middleButton

};
/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initButtonStruct(ButtonStruct* buttonStruct); // alloco staticamente la memoria riservata alla struttura (questa è una alternativa alla allocazione dinamica della memoria riservata alla struttura che in C si fa con malloc)


/**************************************************************************************************************************************/

#endif /* PEN_BUTTON_H */