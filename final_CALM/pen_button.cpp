/*
* pen_button.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_button.h"

void initButtonStruct(ButtonStruct* buttonStruct){

  buttonStruct->leftButton = false;
  buttonStruct->middleButton = false;
  buttonStruct->rightButton = false;

  buttonStruct->lastRightPressTime = 0;
  buttonStruct->lastLeftPressTime = 0;
  buttonStruct->lastCenterPressTime = 0;

  buttonStruct->middleButtonClickCount = 0;
  
}