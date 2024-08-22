/*
* pen_motion.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_motion.h"

void initPenMotionStruct(PenMotionStruct* penMotionStruct){
  
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