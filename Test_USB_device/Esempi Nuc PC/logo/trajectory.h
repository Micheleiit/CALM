#ifndef TRAJECTORY_H
#define TRAJECTORY_H

// Limited by the on-board RAM memory (32KB). Could be improved by recording directly the ticks (relative position, 8bits instead of 32)
// Lasts approx 25 sec
#define TRAJECTORY_MAX_SIZE 365

typedef struct trajectory_calm
{
  // Recorded absolute positions
  int32_t pos_x[TRAJECTORY_MAX_SIZE];
  int32_t pos_y[TRAJECTORY_MAX_SIZE];


  uint16_t current_index;


  uint16_t current_size;
  int current_direction;
  uint32_t current_distance;
  uint32_t total_distance;


  int32_t x_towards;
  int32_t y_towards;

} trajectory_calm;

#endif
