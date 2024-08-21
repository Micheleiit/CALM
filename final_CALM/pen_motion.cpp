/*
* pen_motion.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_motion.h"

void initPenMotionStruct(PenMotionStruct* penmotionst){
  
  penmotionst->delta_roll = 0;
  penmotionst->delta_pitch = 0;

  penmotionst->roll_mm = 0;
  penmotionst->pitch_mm = 0;
  penmotionst->original_roll_mm = 0;
  penmotionst->original_pitch_mm = 0;

  penmotionst->motion = 0;
  penmotionst->restart = false;
  penmotionst->last_time_moved = 0;
  
}