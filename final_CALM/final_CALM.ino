/*
* main.ino
* Created on: August, 2024
* Author: Michele Di Lucchio
*/

/****************************************************************************************************************************************************************
********************************************************************************* INCLUDE **********************************************************************/
#include "app.h"

/***************************************************************************************************************************************************************/

/****************************************************************************************************************************************************************
********************************************************************************* MACRO ************************************************************************/
// Macro for debug prints 
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/***************************************************************************************************************************************************************/


/****************************************************************************************************************************************************************
********************************************************************************* GLOBAL VARIABLES *************************************************************/
uint16_t rx[4];
uint16_t tx[4];

AppStruct myapp;

/***************************************************************************************************************************************************************/

void setup() {

  /************************************* SET-UP SAMD PINS ***************************************/
  
  pinMode(LED_ON_OFF_SWITCH, OUTPUT); // (led verde button - serve per segnalare l'invio di dati)
  pinMode(LED_USER_RED, OUTPUT); // (led rosso sotto - serve per segnalare avviamento codice)
  pinMode(LED_USER_GREEN, OUTPUT); // (led verde sotto - serve per segnalare l'invio di dati)
  pinMode(LED_OK, OUTPUT); // (led rosso frontale - serve per segnalere i movimenti lungo X)
  pinMode(LED_FAULT, OUTPUT); //  (led verde frontale - serve per segnalare i movimenti lungo Y)
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_FAULT, LOW);
  digitalWrite(LED_USER_GREEN, LOW);
  digitalWrite(LED_USER_RED, LOW);
  digitalWrite(LED_ON_OFF_SWITCH, LOW);
  digitalWrite(BUZZER, LOW);

  /********************************* ELENCO DEI MODULI DEL SISTEMA *************************************************/

  ZoomStruct zoom;
  SPIStruct penSpi;
  PenMotionStruct penMotion;
  USBStruct usb;
  ButtonStruct button;
  SPIManager spiManager;

  /************************************* INIZIALIZZAZIONE SPI COMUNICATION ***************************/
  initSPI(&spiManager, IPC_SPI_CS, 100000);

  /*****************************************************************************************/

  initZoomStruct(&zoom);
  initSPIStruct(&penSpi);
  initPenMotionStruct(&penMotion);
  initUSBStruct(&usb);
  initButtonStruct(&button);

  initAppStruct(&myapp, &penMotion, &penSpi, &usb, &zoom, &button, &spiManager); // configuro AppStruct con i puntatori alle strutture di cui sopra

  /************************************* SET-UP USB COMUNICATION ***************************/
  pinMode(nENUSBV, OUTPUT);
  digitalWrite(nENUSBV, LOW); // Enable USB power (Active low)

  /********************************* DEBUG *************************************************/
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  /********************************* TEST-SPI *************************************************/
  digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));
  //slaveReady = testSPICommunication(settings_spi);
  digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));

}

void loop() {
  // put your main code here, to run repeatedly:

}
