/*
* pen_spi.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "pen_spi.h"


void initSPIStruct(SPIStruct* spiStruct){
  
  // variabili per la comunicazione di test
  spiStruct->startTime = 0;
  spiStruct->responseReceived = false;
  spiStruct->tx_test[0] = 0;
  spiStruct->rx_test[0] = 0;
  spiStruct->slaveReady = false;
  
}


void testSPICommunication(SPIStruct* spiStruct, SPIManager* spiManager, uint16_t data){ 

  #ifdef DEBUG
    DEBUG_PRINTLN("Avvio comunicazione di test");
  #endif

  spiStruct->startTime = millis();
  spiStruct->responseReceived = false;
  spiStruct->tx_test[0] = data; // Comando per testare lo stato Init

  delay(500);

    do {

      SPI1.beginTransaction(spiManager->spiSettings);
      digitalWrite(spiManager->csPin, LOW);  // Seleziona lo slave
      spiStruct->rx_test[0] = SPI1.transfer16(spiStruct->tx_test[0]);
      digitalWrite(spiManager->csPin, HIGH); // Deseleziona lo slave
      SPI1.endTransaction();                 // Termina la transazione SPI      

      #ifdef DEBUG
        DEBUG_PRINTLN("");
        DEBUG_PRINTLN("Dati SPI (decimali):");
        DEBUG_PRINT("tx: ");
        DEBUG_PRINT(spiStruct->tx_test[0]);
        DEBUG_PRINT("\t");
        DEBUG_PRINT("rx: ");
        DEBUG_PRINT(spiStruct->rx_test[0]);
        DEBUG_PRINT("\n");
      #endif

      if (spiStruct->rx_test[0] == PEN_INPUT_STATE) {

        digitalWrite(LED_FAULT, LOW);
        digitalWrite(LED_OK, HIGH);

        #ifdef DEBUG
          DEBUG_PRINTLN("");
          DEBUG_PRINTLN("PenInput State.");
        #endif

      } else if (spiStruct->rx_test[0] == INIT_STATE) {

          digitalWrite(LED_OK, LOW);
          digitalWrite(LED_FAULT, HIGH);

          #ifdef DEBUG
            DEBUG_PRINTLN("");
            DEBUG_PRINTLN("Init state");
          #endif

      } else if (spiStruct->rx_test[0] == SPI_READY) {

        digitalWrite(LED_FAULT, LOW);
        digitalWrite(LED_OK, LOW);
        delay(500);
        digitalWrite(LED_FAULT, HIGH);
        digitalWrite(LED_OK, HIGH);
        delay(500);
        digitalWrite(LED_FAULT, LOW);
        digitalWrite(LED_OK, LOW);

        #ifdef DEBUG
          DEBUG_PRINTLN("");
          DEBUG_PRINTLN("SPI ready");
        #endif

        spiStruct->responseReceived = true;
        spiStruct->slaveReady = true;
      }

      if (millis() - spiStruct->startTime > TIMEOUT_DURATION) {
          break;
      }

    } while (!spiStruct->responseReceived);

    if (!spiStruct->responseReceived) {

      spiStruct->slaveReady = false;
      digitalWrite(LED_FAULT, LOW);
      digitalWrite(LED_OK, LOW);

      #ifdef DEBUG
        DEBUG_PRINTLN("Slave not responding. Communication failed.");
      #endif

    }
}