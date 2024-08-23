/*
* pen_motion.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_motion.h"


void initPenMotionStruct(PenMotionStruct* penMotionStruct){
  
  penMotionStruct->rollTX = 0;
  penMotionStruct->pitchTX = 0;
  penMotionStruct->rollRX = 0;
  penMotionStruct->pitchRX = 0;

  
  penMotionStruct->last_time_spi = 0;
  
  penMotionStruct->delta_roll = 0;
  penMotionStruct->delta_pitch = 0;

  penMotionStruct->roll_mm = 0;
  penMotionStruct->pitch_mm = 0;
  penMotionStruct->original_roll_mm = 0;
  penMotionStruct->original_pitch_mm = 0;

  penMotionStruct->motion = 0;
  penMotionStruct->restart = false;
  penMotionStruct->last_time_moved = 0;
  
}


extern PenMotionStruct penMotion; // extern indica al compilatore che la definizione effettiva della variabile o della funzione si trova in un altro file permettendo al file di riconoscere e accedere a variabili e funzioni definite altrove.
extern USBManager usbManager; 

void mouseMoved() {
  // Ottieni i cambiamenti di movimento dal MouseController
  int32_t xChange = usbManager.mouse->getXChange();
  int32_t yChange = usbManager.mouse->getYChange();

  // Aggiorna le variabili delta_roll e delta_pitch
  penMotion.delta_roll = xChange;
  penMotion.delta_pitch = yChange;
  penMotion.motion = 1;

  if(penMotion.motion)
    digitalWrite(LED_OK, HIGH);
}