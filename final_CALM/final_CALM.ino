/*
* main.ino
* Created on: August, 2024
* Author: Michele Di Lucchio
*/

/**********************************************************************************************************************************************************************************
******************************************************************************** INCLUDE *****************************************************************************************/
#include "app.h"

/*********************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************
********************************************************************************* MACRO ******************************************************************************************/

/*********************************************************************************************************************************************************************************/


/**********************************************************************************************************************************************************************************
********************************************************************************** GLOBAL VARIABLES *******************************************************************************/
uint16_t rx[4];
uint16_t tx[4];

// dichiarazione istanze/Oggetti delle strutture

SPIManager spiManager; // oggetto configurazione del protocollo SPI
USBManager usbManager; // oggetto configurazione del protocollo USB

SPIStruct penSpi; // oggetto comunicazione SPI master-sleave
USBStruct penUsb; // oggetto abilitazione host USB

PenMotionStruct penMotion;
ZoomStruct zoom;
ButtonStruct button;

LedBlinkerStruct ledOk;
LedBlinkerStruct ledFault;
LedBlinkerStruct ledOnOff;
LedBlinkerStruct ledRed;
LedBlinkerStruct ledGreen;

AppStruct myapp; // dichiaro un'istanza della strttura AppStruct => sto creando un OGGETTO che rappresenta la APP del mio sistema e che conterrà i valori specifici per ogni campo della struttura APP

/*********************************************************************************************************************************************************************************/

void setup() {

  /********************************************************************************* INIZIALIZZAZIONE MODULI *********************************************************************/
  initSPIManager(&spiManager, IPC_SPI_CS, 100000); 
  initUSBManager(&usbManager);

  initSPIStruct(&penSpi);
  initUSBStruct(&penUsb, nENUSBV);

  initZoomStruct(&zoom);
  initPenMotionStruct(&penMotion);
  initButtonStruct(&button);

  initLedBlinkerStruct(&ledOk, LED_OK, 0); // (led rosso frontale - serve per segnalere i movimenti lungo X)
  initLedBlinkerStruct(&ledFault, LED_FAULT, 0); //  (led verde frontale - serve per segnalare i movimenti lungo Y)
  initLedBlinkerStruct(&ledOnOff, LED_ON_OFF_SWITCH, 0);
  initLedBlinkerStruct(&ledRed, LED_USER_RED, 0); // (led rosso sotto - serve per segnalare avviamento codice)
  initLedBlinkerStruct(&ledGreen, LED_USER_GREEN, 0); // (led verde sotto - serve per segnalare l'invio di dati)

  initAppStruct(&myapp, &spiManager, &usbManager, &penSpi, &penUsb, &penMotion, &zoom, &button, &ledOk, &ledFault, &ledOnOff, &ledRed, &ledGreen); // inizializzo i puntatori alle strutture di cui sopra

  /*********************************************************************************************************************************************************************************/

  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  /********************************* TEST-SPI *************************************************/
  digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));

  // Test della comunicazione SPI all'avvio
  testSPICommunication(&penSpi, &spiManager, 0xAAAA);

  digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));

}

void loop() {
  
  if(myapp.penSpi->slaveReady){

    // work in progress

  } else {

    // Slave non pronto, accendi il buzzer per indicare errore
    digitalWrite(BUZZER, HIGH);

    #ifdef DEBUG
      DEBUG_PRINTLN("");
      DEBUG_PRINTLN("slaveReady FALSE: Inizializzazione fallita!");
    #endif
  }


}
