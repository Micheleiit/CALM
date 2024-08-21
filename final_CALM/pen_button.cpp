/*
* pen_button.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_button.h"

void initButtonStruct(ButtonStruct* buttonst){

  buttonst->leftButton = false;
  buttonst->middleButton = false;
  buttonst->rightButton = false;

  buttonst->lastRightPressTime = 0;
  buttonst->lastLeftPressTime = 0;
  buttonst->lastCenterPressTime = 0;

  buttonst->middleButtonClickCount = 0;
  
}