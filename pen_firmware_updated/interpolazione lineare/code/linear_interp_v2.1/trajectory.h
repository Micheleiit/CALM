/*
* trajectory.h
*
* Created on: September, 2024
*   Author: Michele Di Lucchio
*   Description: implementa una Macchina a Stati Finiti (FSM), una struttura logica che permette al sistema di comportarsi in maniera differente a seconda dello stato corrente in cui si trova 
*/

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

/************************************************************** Define macros *********************************************************/
/**************************************************************************************************************************************/

// Limited by the on-board RAM memory (32KB). Could be improved by recording directly the ticks (relative position, 8bits instead of 32)
// Lasts approx 25 sec
#define TRAJECTORY_MAX_SIZE 1000 //2500
#define STREAMING_PERIOD_ms 1
#define MIN_EUCLID_DIST 4
/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>
#include "led_blinker.h"
/**************************************************************************************************************************************/


/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
typedef struct trajectory_calm
{

  uint16_t current_index; // Tiene traccia della posizione attuale della traiettoria durante l'elaborazione o la riproduzione
  uint16_t current_index_record; // Memorizza l'indice corrente durante la registrazione della traiettoria 

  uint16_t current_size; // Tiene traccia della lunghezza corrente della traiettoria registrata
  int current_direction; //  indica la direzione attuale della riproduzione della traiettoria
  uint32_t current_distance;
  uint32_t total_distance;

  // la posizione verso cui il sistema si sta dirigendo
  int32_t x_towards; 
  int32_t y_towards;

  // Creazione dei vettori di double per gli indici e le coordinate x e y
  double pos_x_double[TRAJECTORY_MAX_SIZE];
  double pos_y_double[TRAJECTORY_MAX_SIZE];
  double index_values[TRAJECTORY_MAX_SIZE];

} trajectory_calm;
/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
trajectory_calm* init_trajectory_struct();
boolean record_trajectory(trajectory_calm* traj, int32_t x_to_be_recorded, int32_t y_to_be_recorded, int32_t min_distance);
void reinit_trajectory(trajectory_calm* traj);
void cleanup_trajectory();
void read_trajectory(trajectory_calm* traj, int32_t* x, int32_t* y, boolean go_back);
boolean read_trajectory_cst_speed(trajectory_calm* traj, int32_t* x, int32_t* y, uint32_t speed_ref, uint16_t scanning_length);

int32_t pow2(int32_t input);

// Dichiarazione del puntatore traj_record come variabile globale accessibile da altri file .cpp
extern trajectory_calm* traj_record;
/**************************************************************************************************************************************/

#endif /* TRAJECTORY_H*/