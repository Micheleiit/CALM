/*
* pen_spi.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_spi.h"

//SPISettings settings_spi(100000, MSBFIRST, SPI_MODE1); // Configura la velocità (1000000 = 1 MHz or 100000 = 100kHz), il formato dei dati (MSBFIRST - Most Significant Bit First) e la modalità SPI (SPI_MODE1).

void initSPIStruct(SPIStruct* spist){
  
  spist->rollTX = 0;
  spist->pitchTX = 0;
  spist->rollRX = 0;
  spist->pitchRX = 0;

  spist->last_time_spi = 0;

  // variabili per la comunicazione di test
  spist->startTime = 0;
  spist->responseReceived = false;
  spist->tx_test[0] = 0;
  spist->rx_test[0] = 0;
  spist->slaveReady = false;

}

/* Test della comunicazione SPI
*  Arguments:
*   -
*   -
*/

bool testSPICommunication(SPISettings settings, SPIStruct* spist){

  #ifdef DEBUG
    DEBUG_PRINTLN("Avvio comunicazione di test");
  #endif

  spist->startTime = millis();

  spist->responseReceived = false;
  spist->tx_test[0] = 0xAAAA;
  spist->rx_test[0] = 0;

  delay(500);

  /*do{

    

  }while ();*/

 
}