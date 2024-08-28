/*
* pen_motion.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_motion.h"

extern PenMotionStruct penMotion; // extern indica al compilatore che la definizione effettiva della variabile o della funzione si trova in un altro file permettendo al file di riconoscere e accedere a variabili e funzioni definite altrove.
extern USBManager usbManager; 
extern SPIManager spiManager;

extern LedBlinkerStruct ledOk; // led giallo frontale
extern LedBlinkerStruct ledFault; // led rosso frontale
extern LedBlinkerStruct ledOnOff; // led bottone 

extern ZoomStruct zoom;

extern uint16_t rx[4];
extern uint16_t tx[4]; 

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

  penMotionStruct->motion = 0;
  penMotionStruct->restart = false;
  penMotionStruct->last_time_moved = 0;
  
}

void sendSPIData(PenMotionStruct* penMotionStruct, SPIManager* spiManager){

  /*switch(zoom.zoom_state) {

    case NORMAL_ZOOM:

      zoom.zoomAmount = 1.0;
      
    break;

    case ZOOM_IN:

      zoom.zoomAmount = zoom.zoomFactor;

    break;

    case ZOOM_OUT:

      zoom.zoomAmount = zoom.zoomFactor; //1.0/zoom.zoomFactor;

    break;

  }*/
  
  // Riscrittura del blocco switch con if e operatore ternario
  if (zoom.zoom_state == NORMAL_ZOOM) {
      zoom.zoomAmount = 1.0;
      setLed(&ledOnOff, HIGH);
  } else {
      zoom.zoomAmount = (zoom.zoom_state == ZOOM_IN || zoom.zoom_state == ZOOM_OUT) ? zoom.zoomFactor : zoom.zoomAmount;
  }

  if(penMotionStruct->restart) { // azzera i dati se è stato premuto a lungo il middle button

    penMotionStruct->rollTX = 0;
    penMotionStruct->pitchTX = 0;
    penMotionStruct->restart = false;
  
  } else {

    penMotionStruct->rollTX += (penMotionStruct->delta_roll * zoom.zoomAmount);
    penMotionStruct->pitchTX += (penMotionStruct->delta_pitch * zoom.zoomAmount);

  }

  // converto gli spostamenti in micrometri 
  penMotionStruct->roll_mm = penMotionStruct->rollTX*1000/30;
  penMotionStruct->pitch_mm = penMotionStruct->pitchTX*1000/30;

  int32_t original_roll_mm = penMotionStruct->roll_mm ;
  int32_t original_pitch_mm = penMotionStruct->pitch_mm;

  // limito il range degli spostamenti
  penMotionStruct->roll_mm = constrain(penMotionStruct->roll_mm, LOWER_BOUND, UPPER_BOUND);
  penMotionStruct->pitch_mm = constrain(penMotionStruct->pitch_mm, LOWER_BOUND, UPPER_BOUND);

  // Resetto i dati ricevuti 
  rx[0] = 0;
  rx[1] = 0;
  rx[2] = 0;
  rx[3] = 0;

  prepareSPIData(penMotionStruct->roll_mm, penMotionStruct->pitch_mm, tx);
  
  SPI1.beginTransaction(spiManager->spiSettings);

  digitalWrite(spiManager->csPin, LOW);     // Seleziona lo slave
  rx[0] = SPI1.transfer16(tx[0]);           // Transfer block 1:  Trasmette un blocco di 16 bit e riceve un blocco di 16 bit simultaneamente
  digitalWrite(spiManager->csPin, HIGH);    // Deseleziona lo slave
  delayMicroseconds(1);

  digitalWrite(IPC_SPI_CS, LOW);
  rx[1] = SPI1.transfer16(tx[1]);           // Transfer block 2
  digitalWrite(IPC_SPI_CS, HIGH);
  delayMicroseconds(1);

  digitalWrite(IPC_SPI_CS, LOW);
  rx[2] = SPI1.transfer16(tx[2]);           // Transfer block 3
  digitalWrite(IPC_SPI_CS, HIGH);
  delayMicroseconds(1);

  digitalWrite(IPC_SPI_CS, LOW);
  rx[3] = SPI1.transfer16(tx[3]);           // Transfer block 4
  digitalWrite(IPC_SPI_CS, HIGH);
  SPI1.endTransaction();   

  //decodeSPIData(penMotionStruct->rollRX, penMotionStruct->pitchRX, rx);

  // Controlla se i valori originali erano fuori range
  if (original_roll_mm > UPPER_BOUND || original_roll_mm < LOWER_BOUND || original_pitch_mm > UPPER_BOUND || original_pitch_mm < LOWER_BOUND) {
    
    // Attiva il buzzer se i valori sono fuori range
    digitalWrite(BUZZER, HIGH);

  }else{

    // Spegni il buzzer se i valori sono all'interno del range
    if (BUZZER) {
      digitalWrite(BUZZER, LOW);
    }

  }

}

void prepareSPIData(int32_t roll, int32_t pitch, uint16_t* txData){

  txData[0] = (int16_t) (0x8000 | (roll & 0xFFF));                  // ROLL LSB -> txData[0] = 0x8XXX
  txData[1] = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));       // ROLL MSB -> txData[1] = 0xCXXX
  txData[2] = (int16_t) (0x0000 | (pitch & 0xFFF));                 // PITCH LSB -> txData[2] = 0x0XXX
  txData[3] = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));      // PITCH MSB -> txData[3] = 0x4XXX
}

// FUNZIONE CHE PERMETTEREBBE DI SFRUTTARE I DATI TRASMESSI DALLO SLAVE VERSO IL MASTER: NON UTILIZZATA IN QUESTA VERSIONE DEL FIRMWARE
/*void decodeSPIData(int32_t data1, int32_t data2, uint16_t* rxData){ 
  data1 = 0;
  data2 = 0;
  data1 |= (0xFFF & rxData[0]);                // Keep only the received 12 lsb (payload) and set them as bits from 0 to 11
  data1 |= (0xFFF & rxData[1]) << 12;          // Keep only the received 12 lsb (payload) and set them as bits from 12 to 23
  data2 |= (0xFFF & rxData[2]);                // Keep only the received 12 lsb (payload) and set them as bits from 0 to 11
  data2 |= (0xFFF & rxData[3]) << 12;

  if (data1 & 0x800000)                        // If bit 23 of updated data is 1 (Negative values)
    data1 |= 0xFF000000;                       // Set to 1 bits from 23 to 31
  else
    data1 &= 0x00FFFFFF;                       // Set to 0 bits from 23 to 31
  if (data2 & 0x800000)                        // If bit 23 of updated data is 1
    data2 |= 0xFF000000;                       // Set to 1 bits from 23 to 31
  else
    data2 &= 0x00FFFFFF;                       // Set to 0 bits from 23 to 31

}*/

/************************************************************************* FUNZIONI WEAK DI MouseController **************************************************************/
void mouseMoved() {
  
  // Ottieni i cambiamenti di movimento dal MouseController
  int32_t xChange = usbManager.mouse.getXChange();
  int32_t yChange = usbManager.mouse.getYChange();

  // Aggiorna le variabili delta_roll e delta_pitch
  penMotion.delta_roll = xChange;
  penMotion.delta_pitch = yChange;
  //penMotion.motion = 1; serviva per gestire i LED => forse posso rimuoverla

  // debug
  //if(penMotion.motion)
    //digitalWrite(LED_OK, HIGH);

  sendSPIData(&penMotion, &spiManager);
}