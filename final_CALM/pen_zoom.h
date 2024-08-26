/*
* pen_motion.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Gestione zoom pennino 
*/

#ifndef PEN_ZOOM_H
#define PEN_ZOOM_H

/************************************************************** Define macros *********************************************************/
/**************************************************************************************************************************************/
#define DEBOUNCE_DELAY 200 // Tempo massimo (in ms) tra pressione e rilascio per considerare un "click" rapido

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>

#include "led_blinker.h"
#include "usb_manager.h"
#include "MouseController.h"
#include "pen_button.h"
/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
enum ZoomState {

  NORMAL_ZOOM = 0,
  ZOOM_IN = 1,
  ZOOM_OUT = 2

};

struct ZoomStruct
{
  float zoomAmount; // rappresenta il livello di zoom normale senza ingrandimenti o riduzioni.
  float zoomFactor;
  ZoomState zoom_state; // zoom_state una variabile di tipo ZoomState che tiene traccia dello stato di zoom del sistema
};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initZoomStruct(ZoomStruct* zoomStruct); // alloco staticamente la memoria riservata alla struttura (questa è una alternativa alla allocazione dinamica della memoria riservata alla struttura che in C si fa con malloc)


/**************************************************************************************************************************************/

#endif /* PEN_ZOOM_H */