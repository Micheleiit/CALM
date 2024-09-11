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
#define DEBOUNCE_DELAY 500 //200 // Tempo massimo (in ms) tra pressione e rilascio per considerare un "click" rapido
#define BLINK_PERIOD 500
#define ZOOM_IN_FACTOR 1 //2
#define ZOOM_OUT_FACTOR 1 //2

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>

#include "MouseController.h"
#include "usb_manager.h"
#include "pen_button.h"
#include "pen_motion.h"
#include "led_blinker.h"
/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
enum ZoomState { // Definisce un tipo di dato enumerato chiamato ZoomState, che rappresenta i possibili stati di zoom in cui il sistema può trovarsi. Ogni stato ha un nome descrittivo

  NORMAL_ZOOM = 0,
  ZOOM_IN = 1,
  ZOOM_OUT = 2,
  UNDEFINED = 3

};

struct ZoomStruct
{
  float zoomAmount; // rappresenta il livello di zoom normale senza ingrandimenti o riduzioni.
  double zoomFactor_new;
  double zoomFactor_old;

  ZoomState zoom_state_new; // zoom_state una variabile di tipo ZoomState che tiene traccia dello stato di zoom del sistema
  ZoomState zoom_state_old;
};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initZoomStruct(ZoomStruct* zoomStruct); // alloco staticamente la memoria riservata alla struttura (questa è una alternativa alla allocazione dinamica della memoria riservata alla struttura che in C si fa con malloc)
void updateZoomState(ZoomState state);

/**************************************************************************************************************************************/

#endif /* PEN_ZOOM_H */