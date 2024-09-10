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
#define SPI_SPEED           100000 // Configura la velocità (1000000 = 1 MHz or 100000 = 100kHz)
#define DATA_SPI_TEST       0xAAAA
#define ERROR_BLINK_PERIOD  500 // ms

/*********************************************************************************************************************************************************************************/

/**********************************************************************************************************************************************************************************
********************************************************************************* GLOBAL VARIABLES ********************************************************************************/

/*dichiarazione istanze delle strutture del sistema. Una istanza di una struttura è un OGGETTO specifico che utilizza il modello definito dalla struttura per contenere dati concreti.
* Una volta creata un'istanza di una struttura, è possibile accedere ai suoi membri e manipolarli come qualsiasi altra variabile.*/

SPIManager spiManager; // oggetto per la configurazione del protocollo SPI
USBManager usbManager; // oggetto per la configurazione del protocollo USB

SPIStruct penSpi; // oggetto per la comunicazione SPI master-sleave
USBStruct penUsb; // oggetto per l' abilitazione dell' host USB (pennino)

PenMotionStruct penMotion;
ZoomStruct zoom;

//ButtonStruct button;
ButtonStruct rightButton;
ButtonStruct leftButton;
ButtonStruct middleButton;

// creazione di più istanze di una struttura, ognuna delle quali contiene i propri dati separati
LedBlinkerStruct ledOk; // led giallo frontale
LedBlinkerStruct ledFault; // led rosso frontale
LedBlinkerStruct ledOnOff; // led bottone
LedBlinkerStruct ledRed; // led rosso sotto
LedBlinkerStruct ledGreen; // led verde sotto

AppStruct myapp; // myapp, istanza della struttura AppStruct, rappresenta l'applicazione nel suo complesso raccogliendo tutte le strutture precedenti per gestire l'intero sistema

// motion related variables
uint16_t rx[4];
uint16_t tx[4];

/*********************************************************************************************************************************************************************************/

void setup() {

  /******************************************************************************* INIZIALIZZAZIONE MODULI ***********************************************************************/
  initSPIManager(&spiManager, IPC_SPI_CS, SPI_SPEED); // inizializzazione del modulo SPI
  initUSBManager(&usbManager); // inizializzazione del modulo USB

  initSPIStruct(&penSpi);
  initUSBStruct(&penUsb, nENUSBV);

  initZoomStruct(&zoom);
  initPenMotionStruct(&penMotion);

  //initButtonStruct(&button);
  initButtonStruct(&rightButton);
  initButtonStruct(&leftButton);
  initButtonStruct(&middleButton);

  // Inizializzazione Buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  initLedBlinkerStruct(&ledOk, LED_OK); // (led rosso frontale - serve per segnalere i movimenti lungo X)
  initLedBlinkerStruct(&ledFault, LED_FAULT); //  (led verde frontale - serve per segnalare i movimenti lungo Y)
  initLedBlinkerStruct(&ledOnOff, LED_ON_OFF_SWITCH);
  initLedBlinkerStruct(&ledRed, LED_USER_RED); // (led rosso sotto - serve per segnalare avviamento codice)
  initLedBlinkerStruct(&ledGreen, LED_USER_GREEN); // (led verde sotto - serve per segnalare l'invio di dati)

  //initAppStruct(&myapp, &spiManager, &usbManager, &penSpi, &penUsb, &penMotion, &zoom, &button, &ledOk, &ledFault, &ledOnOff, &ledRed, &ledGreen); // inizializzo i puntatori alle strutture di cui sopra
  initAppStruct(&myapp, &spiManager, &usbManager, &penSpi, &penUsb, &penMotion, &zoom, &rightButton, &leftButton, &middleButton, &ledOk, &ledFault, &ledOnOff, &ledRed, &ledGreen);
  
  /*********************************************************************************************************************************************************************************/

  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  testSPICommunication(&penSpi, &spiManager, DATA_SPI_TEST); // identificazione degli stati dello slave mandando un messaggio di prova

}

void loop() {
  
  if(myapp.penSpi->slaveReady && myapp.penUsb->USBready){ // slave pronto a processare gli input del pennino

    updateUSB(myapp.usbManager); // richiamo la funzione per eseguire il polling e gestire il dispositivo USB
    
  } else { // Slave non pronto

    //digitalWrite(BUZZER, !digitalRead(BUZZER)); // accendi il buzzer per indicare errore
    errorSignal(myapp.ledOk, myapp.ledFault, ERROR_BLINK_PERIOD);

    #ifdef DEBUG
      DEBUG_PRINTLN("");
      DEBUG_PRINTLN("slaveReady FALSE: Inizializzazione fallita!");
    #endif

  }


}
