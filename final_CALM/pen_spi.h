/*
* pen_spi.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Gestione comuicazione SPI pennino
*/

#ifndef PEN_SPI_H
#define PEN_SPI_H

/************************************************************** Define macros *********************************************************/
/**************************************************************************************************************************************/
#define TIMEOUT_DURATION 60000 // Tempo di timeout in millisecondi per la comunicazione SPI
/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include "pin_map.h"
/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
struct SPIStruct
{
  int32_t rollTX;
  int32_t pitchTX;
  int32_t rollRX;
  int32_t pitchRX;

  unsigned long last_time_spi;
  
  // variabili per la comunicazione di test
  unsigned long startTime;
  bool responseReceived;
  uint16_t tx_test[1];
  uint16_t rx_test[1];
  bool slaveReady;
};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initSPIStruct(SPIStruct* spist);
bool testSPICommunication(SPISettings settings, SPIStruct* spist);


void sendSPIData();
void prepareData(int32_t roll, int32_t pitch, uint16_t* txData);
void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData);
/**************************************************************************************************************************************/

#endif /* PEN_SPI_H */