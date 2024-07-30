// -------------------------------------------------------------- RESET DEL PENNINO FATTO MANUALMENTE DALL'OPERATORE -------------------------------------------------------
#include "pin_map.h"
#include <SPI.h>
#include <MouseController.h> // Includi la libreria per il controllo del mouse

#define COMPILE_SPI           // Se commentato, il programma fa solo lampeggiare alcuni LED
#define LED_OFF_DELAY_ms 20  // La durata (in millisecondi) dopo la quale il LED viene spento se non viene rilevato alcun movimento.
#define UPPER_BOUND 25000
#define LOWER_BOUND -25000

/* Valore inviato dal RM57 quando entra nello stato PenInput per iniziare a ricevere i dati del pennino */
#define TRIGGER_VALUE 100700 // valore esadecimale per 100700 = 0x000188F8   

//#define DEBUG
/* Macro for debug prints */
#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

//*********************************************************************************************************************************************************************//
//************************************************************************ COMUNICAZIONE SPI ***************************************************************************//
//*********************************************************************************************************************************************************************//
/* Preparazione dati e funzioni di comunicazione SPI */
void prepareData(int32_t roll, int32_t pitch, uint16_t* txData);
void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData);

/* Variabili SPI */
SPISettings settings_spi(1000000, MSBFIRST, SPI_MODE1); // Configura la velocità (1 MHz), il formato dei dati (MSBFIRST - Most Significant Bit First) e la modalità SPI (SPI_MODE1).
uint16_t rx[4] = {0};
uint16_t tx[4] = {0};

/* Variabili temporizzazione */
unsigned long last_time_spi = 0;

/*Posizione iniziale del mouse*/
int32_t rollTX = 0;
int32_t pitchTX = 0;

int32_t roll_mm = 0;
int32_t pitch_mm = 0;

bool restart = false;

//*********************************************************************************************************************************************************************//
//************************************************************************ SPOSTAMENTO PENNINO *************************************************************************//
//*********************************************************************************************************************************************************************//
/* Varizioni di posizione del mouse*/
int32_t delta_roll = 0;
int32_t delta_pitch = 0;

uint8_t motion = 0;
unsigned long last_time_moved = 0; // Una variabile che memorizza il tempo (in millisecondi) in cui è stato rilevato l'ultimo movimento del mouse

//*********************************************************************************************************************************************************************//
//************************************************************************ BOTTONI PENNINO ******************************************************************************//
//*********************************************************************************************************************************************************************//
// variables for mouse button states
bool leftButton = false;
bool middleButton = false;
bool rightButton = false;

unsigned long lastRightPressTime = 0;
unsigned long lastLeftPressTime = 0;
unsigned long lastCenterPressTime = 0;
unsigned long debounceDelay = 200; // Tempo massimo (in ms) tra pressione e rilascio per considerare un "click" rapido

// variabile per contare i click del middleButton
unsigned int middleButtonClickCount = 0;

//*********************************************************************************************************************************************************************//
//************************************************************************ ZOOM ***************************************************************************************//
//*********************************************************************************************************************************************************************//
// Variabili di zoom 
float zoomAmount = 0; // rappresenta il livello di zoom normale senza ingrandimenti o riduzioni.
float zoomFactor = 0; // Inizializzazione a 1.0 per evitare problemi con zoomAmount

float* zoomAmountPtr = &zoomAmount; // Puntatore a zoomAmount
float* zoomFactorPtr = &zoomFactor; // Puntatore a zoomAmount

enum state {

  NORMAL_ZOOM = 0,
  ZOOM_IN = 1,
  ZOOM_OUT = 2

};

// Variabile per tenere traccia dello stato di zoom corrente
enum state ZoomState = NORMAL_ZOOM;

//*********************************************************************************************************************************************************************//
//************************************************************************ DATI RM57 ***********************************************************************************//
//*********************************************************************************************************************************************************************//
/* Dati decodificati ricevuti */
int32_t rollRX = 0;
int32_t pitchRX = 0;

//*********************************************************************************************************************************************************************//
//************************************************************************ USB ****************************************************************************************//
//*********************************************************************************************************************************************************************//

// Initialize USB Controller
USBHost usb; // USBHost is the base class for all calls that rely on USB host communication. When invoked, it initializes a USB controller
// Attach mouse controller to USB
MouseController mouse(usb); // MouseController is the class for all calls to the USBHost relating to an attached mouse


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

// This function is called whenever a button on a connected USB mouse is pressed.
void mousePressed() {

  if (mouse.getButton(RIGHT_BUTTON) && !rightButton){

    rightButton = true;
    digitalWrite(LED_USER_GREEN ,HIGH);
    lastRightPressTime = millis();
  }

  if (mouse.getButton(LEFT_BUTTON) && !leftButton){

    leftButton = true;
    digitalWrite(LED_USER_RED, HIGH);
    lastLeftPressTime = millis();
  }

  if (mouse.getButton(MIDDLE_BUTTON) && !middleButton){

    middleButton = true;
    digitalWrite(BUZZER, HIGH);
    lastCenterPressTime = millis();
  }

}

void mouseReleased() {
  
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ RIGHT BUTTON $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

  if (!mouse.getButton(RIGHT_BUTTON) && rightButton) {
    unsigned long elapsedTime = millis() - lastRightPressTime;
    if (elapsedTime <= debounceDelay) { // Click rapido
      
      ZoomState = ZOOM_IN;

      digitalWrite(LED_USER_GREEN, LOW);
      digitalWrite(LED_OK, LOW);
      
    }else{
      ZoomState = NORMAL_ZOOM;
      delay(2000);
      digitalWrite(LED_USER_GREEN, LOW);
    }
    middleButtonClickCount = 0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
    rightButton = false;

  }

  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ LEFT BUTTON $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

  if (!mouse.getButton(LEFT_BUTTON) && leftButton) {
    unsigned long elapsedTime = millis() - lastLeftPressTime;
    if (elapsedTime <= debounceDelay) { // Click rapido
      
      ZoomState = ZOOM_OUT;

      digitalWrite(LED_USER_RED, LOW);
      digitalWrite(LED_FAULT, LOW);
      

    }else{
      ZoomState = NORMAL_ZOOM;
      delay(2000);
      digitalWrite(LED_USER_RED, LOW);
    }
    middleButtonClickCount = 0; // Reset del contatore dei click del middleButton ogni vota che passo da uno stato all'altro
    leftButton = false;

  }

  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ MIDDLE BUTTON $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

  if (!mouse.getButton(MIDDLE_BUTTON) && middleButton) {

    unsigned long elapsedTime = millis() - lastCenterPressTime;
    if (elapsedTime <= debounceDelay+100) { // Click rapido

      /*if (ZoomState == ZOOM_IN) {
        *zoomFactorPtr = 2.0; // fisso il fattore di scala a 2X
      } else if (ZoomState == ZOOM_OUT) {
        *zoomFactorPtr = 2.0; //0.5; (// fisso il fattore di scala a 0.5X)
      }*/
      switch (ZoomState){

        case ZOOM_IN:
        *zoomFactorPtr = 2.0; // fisso il fattore di scala a 2X
        case ZOOM_OUT:
        *zoomFactorPtr = 2.0; //0.5; (// fisso il fattore di scala a 0.5X)

      }
  
      // Incrementa il contatore dei click del middleButton
      middleButtonClickCount++;
      *zoomFactorPtr *= middleButtonClickCount; // Moltiplico il fattore di scala fissato per una costante (n° click middleButton)
      digitalWrite(BUZZER, LOW);
      
      middleButton = false;

    }else{
      
      digitalWrite(BUZZER, LOW);
      
      delay(500);
      // blink dei led
      digitalWrite(LED_OK, HIGH);
      digitalWrite(LED_FAULT, HIGH);
      delay(500);
      digitalWrite(LED_FAULT, LOW);
      delay(500);
      digitalWrite(LED_OK, LOW);
      delay(500);

      middleButton = false;
      restart = true;
      
    }

  }
}

/* Questa funzione viene chiamata quando il mouse si muove */
void mouseMoved() {

  delta_roll = mouse.getXChange();
  delta_pitch = mouse.getYChange();

  motion = 1;
  adjustZoomAmount();

}


void adjustZoomAmount() {
  switch (ZoomState) {

    case NORMAL_ZOOM:
    
      digitalWrite(LED_ON_OFF_SWITCH, HIGH);
      digitalWrite(LED_OK, LOW);
      digitalWrite(LED_FAULT, LOW);

      // Ripristina lo zoom normale
      *zoomAmountPtr = 1.0; 
      *zoomFactorPtr = 1.0; // 0.0

      break; 

    case ZOOM_IN:

        *zoomAmountPtr = zoomFactor;
        digitalWrite(LED_OK, HIGH);
      break;

    case ZOOM_OUT:

        *zoomAmountPtr = 1.0 / zoomFactor; // zoomFactor
        digitalWrite(LED_FAULT, HIGH);

      break;
  }
  sendSPIData();
}


void led_blink() { 

  if(motion != 0){
    if(millis() - last_time_moved >= LED_OFF_DELAY_ms){
      digitalWrite(LED_OK, LOW);
      digitalWrite(LED_FAULT, LOW);
      digitalWrite(LED_ON_OFF_SWITCH, LOW);

    }
    motion = 0;
  }

}

void sendSPIData() {

  #ifdef COMPILE_SPI
    // Resetta i dati ricevuti 
    rx[0] = 0;
    rx[1] = 0;
    rx[2] = 0;
    rx[3] = 0;

    if(restart){ // azzera i dati se è stato premuto a lungo il middle button

      rollTX = 0;
      pitchTX = 0;
      restart = false;
    
    }else{

      /* Questi valori vengono calcolati sommando i cambiamenti di posizione del mouse (mouse.getXChange() e mouse.getYChange()) rispetto alla posizione iniziale (initialX e initialY).*/
      rollTX += (delta_roll * *zoomAmountPtr);    //(delta_roll * zoomAmount); 
      pitchTX += (delta_pitch * *zoomAmountPtr);  // (delta_pitch * zoomAmount);
      
    }

    // converto in micrometri
    roll_mm = rollTX*1000/30;
    pitch_mm = pitchTX*1000/30;

    // Conserva i valori originali per il controllo del buzzer
    int32_t original_roll_mm = roll_mm;
    int32_t original_pitch_mm = pitch_mm;

    // limito il range
    roll_mm = constrain(roll_mm, LOWER_BOUND, UPPER_BOUND);
    pitch_mm = constrain(pitch_mm, LOWER_BOUND, UPPER_BOUND);

    // Prepara i dati da inviare 
    //prepareData(rollTX, pitchTX, tx); // mando i dati in unità di spostamento
    prepareData(roll_mm, pitch_mm, tx); // mando i dati in mm di spostamento

    // Data exchange 
    SPI1.beginTransaction(settings_spi);    // Leave the SPI Data Formats as is on RM57
    digitalWrite(IPC_SPI_CS, LOW);          // Abbassa il Chip Select (CS) per attivare la comunicazione SPI.
    rx[0] = SPI1.transfer16(tx[0]);         // Transfer block 1:  Trasmette un blocco di 16 bit e riceve un blocco di 16 bit simultaneamente
    digitalWrite(IPC_SPI_CS, HIGH);         // Alza il Chip Select (CS) per disattivare la comunicazione SPI temporaneamente
    delayMicroseconds(1);
    digitalWrite(IPC_SPI_CS, LOW);
    rx[1] = SPI1.transfer16(tx[1]);         // Transfer block 2
    digitalWrite(IPC_SPI_CS, HIGH);
    delayMicroseconds(1);
    digitalWrite(IPC_SPI_CS, LOW);
    rx[2] = SPI1.transfer16(tx[2]);         // Transfer block 3
    digitalWrite(IPC_SPI_CS, HIGH);
    delayMicroseconds(1);
    digitalWrite(IPC_SPI_CS, LOW);
    rx[3] = SPI1.transfer16(tx[3]);         // Transfer block 4
    digitalWrite(IPC_SPI_CS, HIGH);
    SPI1.endTransaction();

    // Decode received data 
    decodeData(&rollRX, &pitchRX, rx);

    // Controlla se i valori originali erano fuori range
    if (original_roll_mm > UPPER_BOUND || original_roll_mm < LOWER_BOUND || original_pitch_mm > UPPER_BOUND || original_pitch_mm < LOWER_BOUND) {
        // Attiva il buzzer se i valori sono fuori range
        digitalWrite(BUZZER, HIGH);
        //buzzerActive = true;
    } else {
        // Spegni il buzzer se i valori sono all'interno del range
        if (BUZZER) {
            digitalWrite(BUZZER, LOW);
            //buzzerActive = false;
        }
    }

  #endif
}

void prepareData(int32_t roll, int32_t pitch, uint16_t* txData) {

  txData[0] = (int16_t) (0x8000 | (roll & 0xFFF));                  // ROLL LSB -> txData[0] = 0x8XXX
  txData[1] = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));       // ROLL MSB -> txData[1] = 0xCXXX
  txData[2] = (int16_t) (0x0000 | (pitch & 0xFFF));                 // PITCH LSB -> txData[2] = 0x0XXX
  txData[3] = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));      // PITCH MSB -> txData[3] = 0x4XXX
}

void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData) {
  *data1 = 0;
  *data2 = 0;
  *data1 |= (0xFFF & rxData[0]);                // Keep only the received 12 lsb (payload) and set them as bits from 0 to 11
  *data1 |= (0xFFF & rxData[1]) << 12;          // Keep only the received 12 lsb (payload) and set them as bits from 12 to 23
  *data2 |= (0xFFF & rxData[2]);                // Keep only the received 12 lsb (payload) and set them as bits from 0 to 11
  *data2 |= (0xFFF & rxData[3]) << 12;

  if (*data1 & 0x800000)                        // If bit 23 of updated data is 1 (Negative values)
    *data1 |= 0xFF000000;                     // Set to 1 bits from 23 to 31
  else
    *data1 &= 0x00FFFFFF;                     // Set to 0 bits from 23 to 31
  if (*data2 & 0x800000)                        // If bit 23 of updated data is 1
    *data2 |= 0xFF000000;                     // Set to 1 bits from 23 to 31
  else
    *data2 &= 0x00FFFFFF;                     // Set to 0 bits from 23 to 31
}

/***************************************************************************************************************************************************************************************
********************************************************************************* SETUP ************************************************************************************************
****************************************************************************************************************************************************************************************/

void setup() {

  /************************************* SET-UP LEDS ***************************************/

  pinMode(LED_ON_OFF_SWITCH, OUTPUT); // (led verde button - serve per segnalare l'invio di dati)
  pinMode(LED_USER_RED, OUTPUT); // (led rosso sotto - serve per segnalare avviamento codice)
  pinMode(LED_USER_GREEN, OUTPUT); // (led verde sotto - serve per segnalare l'invio di dati)
  pinMode(LED_OK, OUTPUT); // (led rosso frontale - serve per segnalere i movimenti lungo X)
  pinMode(LED_FAULT, OUTPUT); //  (led verde frontale - serve per segnalare i movimenti lungo Y)

  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_FAULT, LOW);
  digitalWrite(LED_USER_GREEN, LOW);
  digitalWrite(LED_USER_RED, LOW);
  digitalWrite(LED_ON_OFF_SWITCH, LOW);

  /************************************* SET-UP BUZZER *************************************/
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
 
  /************************************* SET-UP SPI COMUNICATION ***************************/

  pinMode(IPC_SPI_CS, OUTPUT);    // SPI Chip Select (SAMD21 funge da master)
  digitalWrite(IPC_SPI_CS, HIGH);

  #ifdef COMPILE_SPI
    SPI1.begin(); //  Inizia una transazione SPI con le impostazioni definite.
  #endif

  /*#ifdef DEBUG
    Serial.begin(115200);
  #endif*/

  /************************************* SET-UP USB COMUNICATION ***************************/
  pinMode(nENUSBV, OUTPUT);
  digitalWrite(nENUSBV, LOW); // Enable USB power (Active low)

  delay(5000); // Un ritardo di 5 secondi viene aggiunto nel setup() per consentire il caricamento del codice prima di inizializzare la gestione USB.

  /* Few seconds delay before main program start */
  for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));
    delay(100);
  }

}

/***************************************************************************************************************************************************************************************
********************************************************************************* LOOP *************************************************************************************************
****************************************************************************************************************************************************************************************/

void loop() {

  usb.Task(); // Process USB tasks: polls connected USB devices for updates to their status
  led_blink();

}






