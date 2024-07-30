#include "trajectory.h"

/*
   Initialization of the structure
*/
trajectory_calm* init_trajectory_struct()
{
  trajectory_calm* traj = (trajectory_calm*) malloc(sizeof(trajectory_calm)); //Important

  traj->current_index = 0;
  traj->current_index_record = 0;
  traj->current_size = 0;
  traj->current_direction = 1;

  traj->x_towards = 0;
  traj->y_towards = 0;
  traj->total_distance = 0;

  traj->current_distance = 0;

  return traj;
}


/*
   Record the trajectory. Stops recording when the structure is full.
   Also computes the distance covered, for not displaying the result if this distance is too small.
*/
void record_trajectory(trajectory_calm* traj, int32_t x_to_be_recorded, int32_t y_to_be_recorded)
{
  if (traj->current_index_record < TRAJECTORY_MAX_SIZE)
  {
    traj->pos_x[traj->current_index_record] = x_to_be_recorded;
    traj->pos_y[traj->current_index_record] = y_to_be_recorded;

    if (traj->current_index_record > 0)
      traj->total_distance += sqrt(pow2(x_to_be_recorded - traj->pos_x[traj->current_index_record - 1]) + pow2(y_to_be_recorded - traj->pos_y[traj->current_index_record - 1]));

    traj->current_index_record += 1;
    traj->current_size += 1;
  }
}

void reinit_trajectory(trajectory_calm* traj)
{
  traj->current_index = 0;
  traj->current_index_record = 0;
  traj->current_size = 0;
  traj->current_direction = 1;
  traj->x_towards = 0;
  traj->y_towards = 0;
  traj->total_distance = 0;
}

/*
   Reads the tabulars in one direction and switch direction when arrived at one extremity.
   When go_back is asserted, it switches in the direction of the most recent point.
*/

void read_trajectory(trajectory_calm* traj, int32_t* x, int32_t* y, boolean go_back)
{
  if (traj->current_index >= traj->current_size)
  {
    traj->current_direction = - 1;
    traj->current_index += - 1;
    traj->current_distance = 0;
  }
  else if (traj->current_index == 0 || go_back)
  {
    traj->current_direction = 1;
    traj->current_distance = 0;
  }

  *x = traj->pos_x[traj->current_index];
  *y = traj->pos_y[traj->current_index];

  traj->current_index += traj->current_direction;
}

/*
   Interpolates the trajectory for covering it at constant speed.
   Scanning length:
   0: Normal operation
   >0: Only covers the specified distance in ticks, at the end of the trajectory.
*/
boolean read_trajectory_cst_speed(trajectory_calm* traj, int32_t* x, int32_t* y, uint32_t speed_ref, uint16_t scanning_length)
{
  uint32_t dist = 0;
  double step_dist = speed_ref * STREAMING_PERIOD_ms / 1000.0;
  boolean go_back;

  if (traj->total_distance > 1000) //If the recorded distance is less than 1000ticks
  {
    if (scanning_length == 0)
    {
      go_back = false; // Abort
    }
    else
    {
      // Shorten the trajectory
      go_back = (traj->current_distance > scanning_length) && (traj->current_direction == -1);
    }

    if (traj->x_towards == 0 && traj->y_towards == 0)
    {
      // If this is the first iteration
      read_trajectory(traj, &traj->x_towards, &traj->y_towards, go_back);
    }
    dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));

    while (dist < step_dist) // While the distance to the next point is less than the required (constant) step distance.
    {
      // Read the next point
      read_trajectory(traj, &traj->x_towards, &traj->y_towards, go_back);
      dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));
    }

    // Go forward, with the specified step distance
    *x = *x + step_dist / dist * (traj->x_towards - *x);
    *y = *y + step_dist / dist * (traj->y_towards - *y);

    traj->current_distance += step_dist;
  }
  else
  {
    return false;
  }
  return true;
}

int32_t pow2(int32_t input)
{
  return input * input;
}
