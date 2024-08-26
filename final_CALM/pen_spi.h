/*
* pen_spi.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Gestione comunicazione di test SPI master-slave
*/

#ifndef PEN_SPI_H
#define PEN_SPI_H

/************************************************************** Define macros *********************************************************/
/**************************************************************************************************************************************/
#define TIMEOUT_DURATION 60000  // Tempo di timeout (in millisecondi) entro cui deve essere stabilita la comunicazione di test SPI
#define INIT_STATE       0x1111 // 4369:  codifica dello stato Init dello slave
#define PEN_INPUT_STATE  0x5555 // 21845: codifica dello stato PenInput dello slave
#define SPI_READY        0x2222 // 8738:  codifica stato SpiReady per iniziare la trasmissione dei dati pennino

#define DEBUG
/* Macro for debug prints */
#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include <Arduino.h>
#include "pin_map.h"

#include "spi_manager.h"

/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
struct SPIStruct
{
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
void initSPIStruct(SPIStruct* spiStruct); // alloco staticamente la memoria riservata alla struttura (questa è una alternativa alla allocazione dinamica della memoria riservata alla struttura che in C si fa con malloc)
void testSPICommunication(SPIStruct* spiStruct, SPIManager* spiManager, uint16_t data);

/**************************************************************************************************************************************/

#endif /* PEN_SPI_H */
