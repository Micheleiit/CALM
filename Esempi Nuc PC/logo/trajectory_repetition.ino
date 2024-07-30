#include "trajectory.h"
//#include "iit.h"
//#include "Pierre_CEYSENS.h"
//#include "Nabeel_Kamal.h"
//#include "Nicolas.h"
//#include "Pierre.h"
#include "cuore.h"
/*
   Initialization of the structure
*/
trajectory_calm* init_trajectory_struct()
{
  trajectory_calm* traj = (trajectory_calm*) malloc(sizeof(trajectory_calm)); //Important

  traj->current_index = 0;


  traj->current_direction = 1;

  traj->x_towards = 0;
  traj->y_towards = 0;
  traj->total_distance = 0;

  traj->current_distance = 0;

//364
  traj->current_size = 103-1;
  return traj;
}



/*
   Reads the tabulars in one direction and switch direction when arrived at one extremity.
   When go_back is asserted, it switches in the direction of the most recent point.
*/

void read_trajectory(trajectory_calm* traj, int32_t* x, int32_t* y)
{
 
  if (traj->current_index >= traj->current_size)
  {
    traj->current_direction = - 1;
    traj->current_index += - 1;
    traj->current_distance = 0;
  }
  else if (traj->current_index == 0)
  {
    traj->current_direction = 1;
    traj->current_distance = 0;
  }

  *x = - pos_x_gcode[traj->current_index] * 70;//2.4;
  *y = pos_y_gcode[traj->current_index] * 70;//2.4;

 traj->current_index += traj->current_direction;
  
}

/*
   Interpolates the trajectory for covering it at constant speed.
   Scanning length:
   0: Normal operation
   >0: Only covers the specified distance in ticks, at the end of the trajectory.
*/
boolean read_trajectory_cst_speed(trajectory_calm* traj, int32_t* x, int32_t* y, uint32_t speed_ref)
{
  uint32_t dist = 0;
  double step_dist = speed_ref * STREAMING_PERIOD_ms / 1000.0;
  if (traj->x_towards == 0 && traj->y_towards == 0)
  {
    // If this is the first iteration
    read_trajectory(traj, &traj->x_towards, &traj->y_towards);
  }
  dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));

  while (dist < step_dist) // While the distance to the next point is less than the required (constant) step distance.
  {
    // Read the next point
    read_trajectory(traj, &traj->x_towards, &traj->y_towards);
    dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));
  }

  // Go forward, with the specified step distance
  *x = *x + step_dist / dist * (traj->x_towards - *x);
  *y = *y + step_dist / dist * (traj->y_towards - *y);

  return true;
}

boolean read_trajectory_var_speed(trajectory_calm* traj, int32_t* x, int32_t* y, uint32_t speed_ref)
{
  uint32_t dist = 0;
  double step_dist;


  if (traj->x_towards == 0 && traj->y_towards == 0)
  {
    // If this is the first iteration
    read_trajectory(traj, &traj->x_towards, &traj->y_towards);
  }
  dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));

  if (traj->current_index > 0 && speed_gcode[traj->current_index-1])
  {
    step_dist = 300000 * STREAMING_PERIOD_ms / 1000.0;
  }
  else
  {
    step_dist = speed_ref * STREAMING_PERIOD_ms / 1000.0;
  }

  while (dist < step_dist) // While the distance to the next point is less than the required (constant) step distance.
  {
    // Read the next point
    read_trajectory(traj, &traj->x_towards, &traj->y_towards);
    dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));

    if (speed_gcode[traj->current_index])
    {
      step_dist = 300000 * STREAMING_PERIOD_ms / 1000.0;
    }
    else
    {
      step_dist = speed_ref * STREAMING_PERIOD_ms / 1000.0;
    }
  }

  // Go forward, with the specified step distance
  *x = *x + step_dist / dist * (traj->x_towards - *x);
  *y = *y + step_dist / dist * (traj->y_towards - *y);

  return true;
}

int32_t pow2(int32_t input)
{
  return input * input;
}
