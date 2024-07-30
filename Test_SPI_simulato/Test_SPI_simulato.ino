//////////// QUESTO CODICE STAMPA I DATI DI ROLL E PITCH SENZA TRASMETTERLI REALMENTE VIA SPI /////////////

#include "pin_map.h"
#include <SPI.h> // The SPI Library is included in every Arduino core/platform, so you do not need to install it externally.

#define DEBUG
#define COMPILE_SPI              // If commented the program only makes some led blink
#define SPI_PERIOD_ms 100       // Delay between each SPI communication

/* Macro for debug prints */
#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif

/* Prepare data and SPI communication functions*/
void prepareData(int counter, uint16_t* txData);
void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData);

/* SPI Variables: The SPISettings object is used to configure the SPI port for your SPI device. All 3 parameters are combined to a single SPISettings object, which is given to SPI.beginTransaction().*/
SPISettings settings_spi(1000000, MSBFIRST, SPI_MODE1);
uint16_t rx[4] = {0};
uint16_t tx[4] = {0};

/* Timing variables */
unsigned long last_time_spi = 0;
int counter = 0;

/* Decoded received data */
int32_t rollRX = 0;
int32_t pitchRX = 0;

/***********************************************************************/

void setup() {
  /*Pin configuration*/
  pinMode(LED_USER_RED,           OUTPUT); // (led rosso sotto)
  pinMode(LED_OK,                 OUTPUT);
  pinMode(LED_FAULT,              OUTPUT);
  pinMode(BUZZER,                 OUTPUT);

  pinMode(IPC_SPI_CS,             OUTPUT);    //SPI Chip Select(SAMD21 acts as the master)
  digitalWrite(IPC_SPI_CS,        HIGH);

#ifdef COMPILE_SPI
  SPI1.begin();
#endif

  Serial.begin(115200);

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
  /* Exchange data over SPI every SPI_PERIOD_ms milliseconds */
  if (millis() >= last_time_spi + SPI_PERIOD_ms)
  {
    /* Update last communication time*/
    last_time_spi = millis();
    /* Toggle red led */
    digitalWrite(LED_FAULT, !digitalRead(LED_FAULT));


#ifdef COMPILE_SPI
    /* Prepare data to be sent */
    prepareData(counter, tx);

    /* Simulate the data reception */
    rx[0] = tx[0];
    rx[1] = tx[1];
    rx[2] = tx[2];
    rx[3] = tx[3];

    /* Rimuoviamo le istruzioni che gestiscono la trasmissione e ricezione dei dati via SPI
    .
    .
    .
    .
    .
     */

    /*Decode received data*/
    decodeData(&rollRX, &pitchRX, rx);

    // Print decoded data in decimal format 
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("RX_ROLL = TX_ROLL e RX_PITCH = TX_PITCH decimali:");
    DEBUG_PRINT("Roll: ");
    DEBUG_PRINT(rollRX);  DEBUG_PRINT("\t");
    DEBUG_PRINT("Pitch: ");
    DEBUG_PRINT(pitchRX);  DEBUG_PRINT("\n");

    /* Print data in hexadecimal format */
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("ROLL e PITCH spacchettati (2 x 16 bit):");
    DEBUG_PRINT("Tx_Roll : 0x"); DEBUG_PRINT(tx[1], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINT(tx[0], HEX); DEBUG_PRINT("  Rx_Roll : 0x"); DEBUG_PRINT(rx[1], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(rx[0], HEX);
    DEBUG_PRINT("Tx_Pitch: 0x"); DEBUG_PRINT(tx[3], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINT(tx[2], HEX); DEBUG_PRINT("  RX_Pitch: 0x"); DEBUG_PRINT(rx[3], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(rx[2], HEX);
 
    // Stampa dei valori decodificati in formato esadecimale, con padding a 8 cifre
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Valori di ROLL e PITCH ricostruiti:");
    char buffer[9];  // Buffer per contenere la stringa esadecimale con terminatore nullo
    sprintf(buffer, "%08X", (uint32_t)rollRX);
    DEBUG_PRINT("Roll (hex): 0x"); DEBUG_PRINT(buffer); DEBUG_PRINT("\t");
    sprintf(buffer, "%08X", (uint32_t)pitchRX);
    DEBUG_PRINT("Pitch (hex): 0x"); DEBUG_PRINT(buffer); DEBUG_PRINT("\n");

   /*//Uncomment to print the single received words
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("I valori grezzi dei dati rx:");
    DEBUG_PRINT(rx[0]);  DEBUG_PRINT("\t");
    DEBUG_PRINT(rx[1]);  DEBUG_PRINT("\t");
    DEBUG_PRINT(rx[2]);  DEBUG_PRINT("\t");
    DEBUG_PRINT(rx[3]);  DEBUG_PRINT("\n");*/
#endif

    /* Counter Update */
    counter++;
  }
}

void prepareData(int counter, uint16_t* txData) {

  /* Nella funzione prepareData, ogni dato a 32 bit (roll e pitch) viene suddiviso in due parole da 12 bit. Tuttavia, questi 12 bit sono poi inseriti in parole da 16 bit (uint16_t), 
  con i restanti 4 bit utilizzati come prefissi per indicare il tipo di dato. */

  //int32_t roll = -10000 + counter;
  //int32_t pitch = 20000 + counter;

  int32_t roll =  1;
  int32_t pitch = 1;

  txData[0] = (int16_t) (0x8000 | (roll & 0xFFF));                  // ROLL LSB: estraiamo i 12 bit meno significativi del valore di roll (XXX = payload) -> 0x8000 è il prefisso per indicare che questa parola contiene i bit meno significativi di roll -> Questi due valori vengono combinati usando l'operatore OR (|) per formare una parola da 16 bit del tipo 0x8XXX
  txData[1] = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));       // ROLL MSB: estraiamo i 12 bit successivi del valore di roll (XXX = payload), li spostiamo a destra di 12 bit -> 0xC000 è il prefisso per indicare che questa parola contiene i bit più significativi di roll -> Questi due valori vengono combinati usando l'operatore OR (|) per formare una parola da 16 bit deltipo 0xCXXX

  // Il valore di pitch viene trattato in modo analogo a roll
  txData[2] = (int16_t) (0x0000 | (pitch & 0xFFF));                 // PITCH LSB: estraiamo i 12 bit meno significativi del valore di pitch (XXX = payload) -> 0x0000 è il prefisso per indicare che questa parola contiene i bit meno significativi di pitch -> Questi due valori vengono combinati usando l'operatore OR (|) per formare una parola da 16 bit deltipo 0x0XXX
  txData[3] = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));      // PITCH MSB: estraiamo i 12 bit successivi del valore di pitch (XXX = payload), li spostiamo a destra di 12 bit -> 0x4000 è il prefisso per indicare che questa parola contiene i bit più significativi di pitch -> Questi due valori vengono combinati usando l'operatore OR (|) per formare una parola da 16 bit deltipo 0x4XXX
}

void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData) { /* La funzione decodeData ha lo scopo di ricostruire i valori originali di rollio (roll) e beccheggio (pitch) a partire dai dati 
ricevuti tramite SPI. Questi dati sono stati precedentemente suddivisi in segmenti da 12 bit e combinati con prefissi specifici per la trasmissione */
  *data1 = 0;
  *data2 = 0;

  *data1 |= (0xFFF & rxData[0]);                // estrae i 12 bit meno significativi di rxData[0] (ovvero il payload di ROLL LSB) e li assegna ai bit da 0 a 11 di data1
  *data1 |= (0xFFF & rxData[1]) << 12;          // estrae i 12 bit meno significativi di rxData[1] (ovvero il payload di ROLL MSB) e li sposta a sinistra di 12 bit, quindi li assegna ai bit da 12 a 23 di data1

  *data2 |= (0xFFF & rxData[2]);                // estrae i 12 bit meno significativi di rxData[2] (LSB di pitch ovvero il payload) e li assegna ai bit da 0 a 11 di data2
  *data2 |= (0xFFF & rxData[3]) << 12;          // estrae i 12 bit meno significativi di rxData[3] (MSB di pitch) e li sposta a sinistra di 12 bit, quindi li assegna ai bit da 12 a 23 di data2

  if (*data1 & 0x800000)                          //If bit 23 of updated data is 1    (Negative values)
    *data1 |= 0xFF000000;                         //Set to 1 bits from 23 to 31
  else
    *data1 &= 0x00FFFFFF;                         //Set to 0 bits from 23 to 31
  if (*data2 & 0x800000)                          //If bit 23 of updated data is 1    
    *data2 |= 0xFF000000;                         //Set to 1 bits from 23 to 31
  else
    *data2 &= 0x00FFFFFF;                         //Set to 0 bits from 23 to 31
}

