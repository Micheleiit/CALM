/*
* pen_motion.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Gestione movimento pennino 
*/

#ifndef PEN_MOTION_H
#define PEN_MOTION_H

/************************************************************** Define macros *********************************************************/
/**************************************************************************************************************************************/
#define LED_OFF_DELAY_ms 20  // La durata (in millisecondi) dopo la quale il LED viene spento se non viene rilevato alcun movimento.
#define UPPER_BOUND 25000
#define LOWER_BOUND -25000 
/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>
#include "usb_manager.h"

// #include "pin_map.h" // debug
/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
struct PenMotionStruct
{

  int32_t rollTX;
  int32_t pitchTX;
  int32_t rollRX;
  int32_t pitchRX;

  unsigned long last_time_spi;

  int32_t delta_roll; // Spostamento del mouse lungo X
  int32_t delta_pitch; // Spostamento del mouse lungo Y

  int32_t roll_mm;
  int32_t pitch_mm;
  int32_t original_roll_mm;
  int32_t original_pitch_mm;

  uint8_t motion;
  bool restart;
  unsigned long last_time_moved; // Una variabile che memorizza il tempo (in millisecondi) in cui è stato rilevato l'ultimo movimento del mouse

};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initPenMotionStruct(PenMotionStruct* penMotionStruct); // alloco staticamente la memoria riservata alla struttura (questa è una alternativa alla allocazione dinamica della memoria riservata alla struttura che in C si fa con malloc)


//void prepareData(int32_t roll, int32_t pitch, uint16_t* txData);
//void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData);

/**************************************************************************************************************************************/

#endif /* PEN_MOTION_H */