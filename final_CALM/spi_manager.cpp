/*
* spi_manager.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "spi_manager.h"

void initSPI(SPIManager* spi, int csPin, uint32_t spiSpeed){
  
  spi->csPin = csPin;
  spi->spiSpeed = spiSpeed;
  spi->spiSettings = SPISettings(spiSpeed, MSBFIRST, SPI_MODE1);

  pinMode(spi->csPin, OUTPUT); // SPI Chip Select (SAMD21 funge da master)
  digitalWrite(spi->csPin, HIGH); // deseleziona lo slave
  SPI1.begin();

}
