/*
* spi_manager.h
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*   Description: Settaggio protocollo di comunicazione SPI 
*/

#ifndef SPI_MANAGER_H
#define SPI_MANAGER_H

/************************************************************* Define Macros **********************************************************/
/**************************************************************************************************************************************/

/**************************************************************************************************************************************/

/************************************************************* Include Files **********************************************************/
/**************************************************************************************************************************************/
#include "Arduino.h"
#include <SPI.h>

/**************************************************************************************************************************************/

/************************************************************* Type Definitions *******************************************************/
/**************************************************************************************************************************************/
struct SPIManager
{

  int csPin;               // pin di selezione per lo slave
  uint32_t spiSpeed;       // velocità della comunicazione SPI
  SPISettings spiSettings;
  
};

/**************************************************************************************************************************************/

/************************************************************* Function Declarations **************************************************/
/**************************************************************************************************************************************/
void initSPIManager(SPIManager* spiManager, int csPin, uint32_t spiSpeed);
//uint16_t spiTransfer(SPIManager* spi, uint16_t data); WORK IN PROGRESS........

/**************************************************************************************************************************************/



#endif /* SPI_MANAGER_H */