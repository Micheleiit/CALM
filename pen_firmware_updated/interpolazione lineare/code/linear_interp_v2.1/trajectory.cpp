/*
* trajectory.cpp
*
* Created on: September, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "trajectory.h"

extern LedBlinkerStruct ledGreen; // led verde sotto

// Definizione del puntatore globale alla struttura trajectory_calm
trajectory_calm* traj_record = NULL;  // Inizializza il puntatore globale a NULL: non punta a nessun blocco di memoria

// ***********************************************************
// *********** INZIALIZZAZIONE DELLA STRUTTURA  **************
// ***********************************************************
trajectory_calm* init_trajectory_struct()
{
  trajectory_calm* traj_record = (trajectory_calm*) malloc(sizeof(trajectory_calm)); // Allocazione di memoria

  if (traj_record != NULL){ // aggiorno il valore del puntatore traj_record con un nuovo indirizzo di memoria che ora punta al blocco allocato.

    traj_record->current_index = 0;
    traj_record->current_index_record = 0;
    traj_record->current_size = 0;
    traj_record->current_direction = 1;

    traj_record->x_towards = 0;
    traj_record->y_towards = 0;
    traj_record->total_distance = 0;

    traj_record->current_distance = 0;

    // Inizializza i vettori delle coordinate double (pos_x_double, pos_y_double)
    memset(traj_record->pos_x_double, 0, sizeof(traj_record->pos_x_double));
    memset(traj_record->pos_y_double, 0, sizeof(traj_record->pos_y_double));

    // Inizializza i valori degli indici (index_values)
    memset(traj_record->index_values, 0, sizeof(traj_record->index_values));

    return traj_record; // Restituisce il puntatore alla struttura inizializzata

  } else {

    return NULL;
  }
  
}

// ***********************************************************
// *********** REGISTRAZIONE DELLA TRAIETTORIA  **************
// ***********************************************************
/*
Record the trajectory. Stops recording when the structure is full.
Also computes the distance covered, for not displaying the result if this distance is too small.
*/
boolean record_trajectory(trajectory_calm* traj, int32_t x_to_be_recorded, int32_t y_to_be_recorded, int32_t min_distance)
{
  if (traj->current_index_record < TRAJECTORY_MAX_SIZE) { // controllo per evitare overflow
      
    // Se ci sono già punti registrati, controlla la distanza rispetto all'ultimo nodo
    if (traj->current_index_record > 0) {

      // Calcola la distanza euclidea tra il nuovo punto e l'ultimo nodo registrato
      int32_t dx = x_to_be_recorded - traj->pos_x_double[traj->current_index_record - 1];
      int32_t dy = y_to_be_recorded - traj->pos_y_double[traj->current_index_record - 1];
      int32_t dist = sqrt(pow2(dx) + pow2(dy));

      // Se la distanza è inferiore a min_distance, non registrare il punto
      if (dist < min_distance) {
        return false;
      }
    }

    // Se la distanza è sufficiente, memorizza il nuovo nodo
    traj->pos_x_double[traj->current_index_record] = static_cast<double>(x_to_be_recorded); // Conversione dei valori di x in double
    traj->pos_y_double[traj->current_index_record] = static_cast<double>(y_to_be_recorded); // Conversione dei valori di y in double

    // Calcola la distanza totale (solo se ci sono già punti registrati)
    if (traj->current_index_record > 0) {
      traj->total_distance += sqrt(pow2(x_to_be_recorded - traj->pos_x_double[traj->current_index_record - 1]) + 
                                    pow2(y_to_be_recorded - traj->pos_y_double[traj->current_index_record - 1]));
    }

    // Registra l'indice
    traj->index_values[traj->current_index_record] = traj->current_index_record;

    // Aggiorna gli indici
    traj->current_index_record += 1;
    traj->current_size += 1; // aggiorna la dimensione del vettore

  } else {
    setLed(&ledGreen, !ledGreen.isOn); // Indicazione di overflow della traiettoria
  }

  return true;
  
}

// ***********************************************************
// ********** REINIZIALIZZAZIONE DELLA TRAIETTORIA  **********
// ***********************************************************
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

void cleanup_trajectory() {

  if (traj_record != NULL) {
    free(traj_record);  // Libera la memoria dinamica allocata per la traiettoria
    traj_record = NULL; // Previene l'uso accidentale di un puntatore non valido
  }

}

// ***********************************************************
// *********** LETTURA DELLA TRAIETTORIA REGISTRATA **********
// ***********************************************************
/*
Reads the tabulars in one direction and switch direction when arrived at one extremity.
When go_back is asserted, it switches in the direction of the most recent point.
*/

void read_trajectory(trajectory_calm* traj, int32_t* x, int32_t* y, boolean go_back)
{
  if (traj->current_index >= traj->current_size -1) // se si è raggiunta la fine della traiettoria
  {
    traj->current_direction = - 1;
    //traj->current_index += - 1;
    //traj->current_distance = 0;
    //Serial.println("back");
  }
  else if (traj->current_index == 0 || go_back) // cambia direnzione di scorrimento del vettore in cui è salvata la traiettoria registrata
  {
    traj->current_direction = 1;
    //traj->current_distance = 0;
    //Serial.println("forward");
  }
  
  // assegno le coordinate del punto corrente alle variabili puntate da x e y
  *x = traj->pos_x_double[traj->current_index];
  *y = traj->pos_y_double[traj->current_index];

  traj->current_index += traj->current_direction; // aggiorno l'indice corrente in base alla direzione attuale
}

// ***********************************************************
// *********** INTERPOLA LA TRAIETTORIA REGISTRATA ***********
// ***********************************************************
/*
Interpolates the trajectory for covering it at constant speed.
Scanning length:
0: Normal operation
>0: Only covers the specified distance in ticks, at the end of the trajectory.
*/
boolean read_trajectory_cst_speed(trajectory_calm* traj, int32_t* x, int32_t* y, uint32_t speed_ref, uint16_t scanning_length)
{
  uint32_t dist = 0; // distanza fra il punto corrente e il punto successivo
  double step_dist = speed_ref * STREAMING_PERIOD_ms / 1000.0; // calcola gli step da percorerre fra ogni coppia di punti di controllo in base alla velocità desiderata (speed_ref)
  boolean go_back;

  if (traj->total_distance > 1000) // Se la distanza totale registrata è inferiore a 1000 ticks, la traiettoria è considerata troppo breve e la funzione restituisce false per indicare che il movimento deve essere interrotto.
  {
    if (scanning_length == 0) //  non viene effettuata nessuna scansione parziale
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
      // se siamo alla prima iterazione => chiamiamo read_trajectory() per leggere il primo punto obiettivo lungo la traiettoria
      read_trajectory(traj, &traj->x_towards, &traj->y_towards, go_back);
    }

    dist = sqrt((float) (pow2(traj->x_towards - *x) + pow2(traj->y_towards - *y)));

    while (dist < step_dist) // While the distance to the next point is less than the required (constant) step distance.
    {
      // Read the next point: Qui si entra sempre dopo la lettura del primo punto della traiettoria dato che dist = 0
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
    return false; // Non ci sono abbastanza dati per procedere
  }
  return true;
}

int32_t pow2(int32_t input)
{
  return input * input;
}
