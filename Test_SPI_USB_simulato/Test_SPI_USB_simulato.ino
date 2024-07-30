// Con questo codice vogliamo essere in grado di costruire e trasmettere via SPI dei pacchetti di dati relativi alla posizione del pennino //

#include "pin_map.h"
#include <SPI.h>
#include <MouseController.h> // Includi la libreria per il controllo del mouse

#define DEBUG
#define COMPILE_SPI              // Se commentato, il programma fa solo lampeggiare alcuni LED
#define SPI_PERIOD_ms 100       // Ritardo tra ogni comunicazione SPI

/* Macro per debug prints */
#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif

/* Preparazione dati e funzioni di comunicazione SPI */
void prepareData(int counter, uint16_t* txData);
void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData);

/* Variabili SPI */
SPISettings settings_spi(1000000, MSBFIRST, SPI_MODE1);
uint16_t rx[4] = {0};
uint16_t tx[4] = {0};

/* Variabili temporizzazione */
unsigned long last_time_spi = 0;
int counter = 0;

/* Dati decodificati ricevuti */
int32_t rollRX = 0;
int32_t pitchRX = 0;

// Initialize USB Controller
USBHost usb; // USBHost is the base class for all calls that rely on USB host communication. When invoked, it initializes a USB controller

// Attach mouse controller to USB
MouseController mouse(usb); // MouseController is the class for all calls to the USBHost relating to an attached mouse

/***********************************************************************/

void setup() {
  /* Configurazione pin */
  pinMode(LED_USER_RED,           OUTPUT); // (led rosso sotto)
  pinMode(LED_OK,                 OUTPUT);
  pinMode(LED_FAULT,              OUTPUT);
  pinMode(BUZZER,                 OUTPUT);

  pinMode(IPC_SPI_CS,             OUTPUT);    // SPI Chip Select (SAMD21 funge da master)
  digitalWrite(IPC_SPI_CS,        HIGH);

#ifdef COMPILE_SPI
  SPI1.begin();
#endif

  Serial.begin(115200);

  /* Inizializza USB Host */
  //usb.begin();

  /*Few seconds delay before main program start*/
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));
    delay(100);
  }
}

/***********************************************************************/

void loop() {
  /* Aggiorna la gestione del mouse USB */
  usb.Task();

  /* Se è passato SPI_PERIOD_ms millisecondi, esegui la comunicazione SPI */
  if (millis() >= last_time_spi + SPI_PERIOD_ms) {
    /* Aggiorna l'ultima ora di comunicazione */
    last_time_spi = millis();
    /* Alterna il LED rosso */
    digitalWrite(LED_FAULT, !digitalRead(LED_FAULT));

#ifdef COMPILE_SPI
    /* Ottieni i dati del mouse */
    int32_t roll = mouse.getXChange();  // Ottieni il cambiamento di posizione sull'asse X
    int32_t pitch = mouse.getYChange(); // Ottieni il cambiamento di posizione sull'asse Y

    /* Prepara i dati da inviare */
    prepareData(roll, pitch, tx);

    /* Simula la ricezione dei dati */
    rx[0] = tx[0];
    rx[1] = tx[1];
    rx[2] = tx[2];
    rx[3] = tx[3];

    /* Decodifica i dati ricevuti */
    decodeData(&rollRX, &pitchRX, rx);

    /* Stampa i dati in formato decimale */
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("RX_ROLL = TX_ROLL e RX_PITCH = TX_PITCH decimali:");
    DEBUG_PRINT("Roll: ");
    DEBUG_PRINT(rollRX);  DEBUG_PRINT("\t");
    DEBUG_PRINT("Pitch: ");
    DEBUG_PRINT(pitchRX);  DEBUG_PRINT("\n");

    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("ROLL e PITCH spacchettati (2 x 16 bit):");
    DEBUG_PRINT("Tx_Roll : 0x"); DEBUG_PRINT(tx[1], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINT(tx[0], HEX); DEBUG_PRINT("  Rx_Roll : 0x"); DEBUG_PRINT(rx[1], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(rx[0], HEX);
    DEBUG_PRINT("Tx_Pitch: 0x"); DEBUG_PRINT(tx[3], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINT(tx[2], HEX); DEBUG_PRINT("  RX_Pitch: 0x"); DEBUG_PRINT(rx[3], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(rx[2], HEX);

    /* Stampa dei valori decodificati in formato esadecimale, con padding a 8 cifre */
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Valori di ROLL e PITCH ricostruiti:");
    char buffer[9];  // Buffer per contenere la stringa esadecimale con terminatore nullo
    sprintf(buffer, "%08X", (uint32_t)rollRX);
    DEBUG_PRINT("Roll (hex): 0x"); DEBUG_PRINT(buffer); DEBUG_PRINT("\t");
    sprintf(buffer, "%08X", (uint32_t)pitchRX);
    DEBUG_PRINT("Pitch (hex): 0x"); DEBUG_PRINT(buffer); DEBUG_PRINT("\n");

    /* Aggiorna il contatore */
    counter++;
#endif
  }
}

void prepareData(int32_t roll, int32_t pitch, uint16_t* txData) {
  txData[0] = (int16_t) (0x8000 | (roll & 0xFFF));                  // ROLL LSB
  txData[1] = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));       // ROLL MSB
  txData[2] = (int16_t) (0x0000 | (pitch & 0xFFF));                 // PITCH LSB
  txData[3] = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));      // PITCH MSB
}

void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData) {
  *data1 = 0;
  *data2 = 0;

  *data1 |= (0xFFF & rxData[0]);
  *data1 |= (0xFFF & rxData[1]) << 12;

  *data2 |= (0xFFF & rxData[2]);
  *data2 |= (0xFFF & rxData[3]) << 12;

  if (*data1 & 0x800000)
    *data1 |= 0xFF000000;
  else
    *data1 &= 0x00FFFFFF;
  if (*data2 & 0x800000)
    *data2 |= 0xFF000000;
  else
    *data2 &= 0x00FFFFFF;
}
