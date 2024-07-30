#include "pin_map.h"
#include <SPI.h>
#include <MouseController.h> // Includi la libreria per il controllo del mouse

#define COMPILE_SPI              // Se commentato, il programma fa solo lampeggiare alcuni LED

#define LED_OFF_DELAY_ms 20  // La durata (in millisecondi) dopo la quale il LED viene spento se non viene rilevato alcun movimento.

/* Preparazione dati e funzioni di comunicazione SPI */
void prepareData(int32_t roll, int32_t pitch, uint16_t* txData);
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

/* Dati da inviare */
int32_t roll = 0;
int32_t pitch = 0;
int32_t rollTX = 0;
int32_t pitchTX = 0;

uint8_t motion = 0;
unsigned long last_time_moved = 0; // Una variabile che memorizza il tempo (in millisecondi) in cui è stato rilevato l'ultimo movimento del mouse

// Initialize USB Controller
USBHost usb; // USBHost is the base class for all calls that rely on USB host communication. When invoked, it initializes a USB controller

// Attach mouse controller to USB
MouseController mouse(usb); // MouseController is the class for all calls to the USBHost relating to an attached mouse

/* Posizione iniziale del mouse */
int initialX = 0;
int initialY = 0;

/* Questa funzione viene chiamata quando il mouse si muove */
void mouseMoved() {

  roll = initialX + mouse.getXChange();
  pitch = initialY + mouse.getYChange();

  led_blink();

  motion = 1;

}

void led_blink() { // Accendi i LED in base al movimento

  if (roll != 0) {
    digitalWrite(LED_OK, HIGH);
    last_time_moved = millis();
  }

  if (pitch != 0) {
    digitalWrite(LED_FAULT, HIGH);
    last_time_moved = millis();
  }

  sendSPIData();
}

void sendSPIData() {

  #ifdef COMPILE_SPI
    // Resetta i dati ricevuti 
    rx[0] = 0;
    rx[1] = 0;
    rx[2] = 0;
    rx[3] = 0;

    /* Questi valori vengono calcolati sommando i cambiamenti di posizione del mouse (mouse.getXChange() e mouse.getYChange()) rispetto alla posizione iniziale (initialX e initialY).*/
    rollTX += roll;
    pitchTX += pitch;

    // Prepara i dati da inviare 
    prepareData(rollTX, pitchTX, tx);

    digitalWrite(LED_USER_GREEN, HIGH);

    // Data exchange 
    SPI1.beginTransaction(settings_spi);    //Leave the SPI Data Formats as is on RM57
    digitalWrite(IPC_SPI_CS, LOW);          // Trigger CS (active low): begin of transmission
    rx[0] = SPI1.transfer16(tx[0]);         // Transfer block 1
    digitalWrite(IPC_SPI_CS, HIGH);         // End of transmission
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

  #endif
}



void setup() {
  /* Configurazione pin */
  pinMode(LED_USER_RED, OUTPUT); // (led rosso sotto - serve per segnalare avviamento codice)
  pinMode(LED_USER_GREEN, OUTPUT); // (led verde sotto - serve per segnalare l'invio di dati)
  pinMode(LED_OK, OUTPUT); // (led rosso frontale - serve per segnalere i movimenti lungo X)
  pinMode(LED_FAULT, OUTPUT); //  (led verde frontale - serve per segnalare i movimenti lungo Y)
  pinMode(BUZZER, OUTPUT);
  pinMode(nENUSBV, OUTPUT);
  pinMode(IPC_SPI_CS, OUTPUT);    // SPI Chip Select (SAMD21 funge da master)

  digitalWrite(IPC_SPI_CS, HIGH);
  digitalWrite(nENUSBV, LOW); // Enable USB power (Active low)

  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_FAULT, LOW);
  digitalWrite(LED_USER_GREEN, LOW);


  #ifdef COMPILE_SPI
    SPI1.begin();
  #endif

  delay(5000); // Un ritardo di 5 secondi viene aggiunto nel setup() per consentire il caricamento del codice prima di inizializzare la gestione USB.

  /* Few seconds delay before main program start */
  for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    digitalWrite(LED_USER_RED, !digitalRead(LED_USER_RED));
    delay(100);
  }

}

void loop() {

  if(motion != 0){
    if(millis() - last_time_moved >= LED_OFF_DELAY_ms){
      digitalWrite(LED_OK, LOW);
      digitalWrite(LED_FAULT, LOW);
      digitalWrite(LED_USER_GREEN, LOW);
    }
  }

  usb.Task(); // Process USB tasks: polls connected USB devices for updates to their status

}

void prepareData(int32_t roll, int32_t pitch, uint16_t* txData) {

  //digitalWrite(LED_USER_GREEN, !digitalRead(LED_USER_GREEN));

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

