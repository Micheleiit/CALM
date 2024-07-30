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
#endif

/* Prepare data and SPI communication functions*/
void prepareData(int counter, uint16_t* txData, int32_t* add1, int32_t* add2);
//void prepareData(int counter, uint16_t* txData);
void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData);

/* SPI Variables: The SPISettings object is used to configure the SPI port for your SPI device. All 3 parameters are combined to a single SPISettings object, which is given to SPI.beginTransaction().*/
SPISettings settings_spi(1000000, MSBFIRST, SPI_MODE1);
uint16_t rx[4] = {0}; // array per i dati RICEVUTI
uint16_t tx[4] = {0}; // array per i dati TRASMESSI

/* Timing variables */
unsigned long last_time_spi = 0; // tengo traccia dell'ultimo istante temporale in cui è avvenuto uno scambio di dati SPI
int counter = 0;

/* Decoded received data */
int32_t rollRX = 0; // memorizzo i dati ricevuti di ROLL
int32_t pitchRX = 0; // memorizzo i dati ricevuti di PITCH

/* Dati da inviare*/
int32_t roll = 0;
int32_t pitch = 0;

bool ready_to_sent = false;

/***********************************************************************/

void setup() {


  /*Pin configuration*/
  pinMode(LED_USER_RED,           OUTPUT); // (led rosso sotto)
  pinMode(LED_FAULT,              OUTPUT); // (led rosso frontale)
  pinMode(BUZZER,                 OUTPUT);
  digitalWrite(LED_USER_RED, LOW);
  digitalWrite(LED_FAULT, LOW);
  digitalWrite(BUZZER, LOW);

  pinMode(IPC_SPI_CS,             OUTPUT);    //SPI Chip Select(SAMD21 acts as the master)
  digitalWrite(IPC_SPI_CS,        HIGH);

#ifdef COMPILE_SPI
  /* On CALM board, SAMD21 is using two SERCOMs as SPI interfaces. For the communication with RM57 we need to use SPI on SERCOM1
      According to the definitions provided in the Calm variant.h file, we need to use SPI1 instead of SPI in order to control the
      desired peripheral through the SPI.h library*/
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
    /* Reset received data */
    rx[0] = 0; 
    rx[1] = 0; 
    rx[2] = 0; 
    rx[3] = 0;

    /* Prepare data to be sent */
    //prepareData(counter, tx);
    prepareData(counter, tx, &roll, &pitch);

    

    /* Data exchange */
    SPI1.beginTransaction(settings_spi);    //Leave the SPI Data Formats as is on RM57
    digitalWrite(IPC_SPI_CS, LOW);          // Trigger CS (active low): begin of transmission
    rx[0] = SPI1.transfer16(tx[0]);      // Transfer block 1
    digitalWrite(IPC_SPI_CS, HIGH);         // End of transmission
    delayMicroseconds(1);
    digitalWrite(IPC_SPI_CS, LOW);
    rx[1] = SPI1.transfer16(tx[1]);      // Transfer block 2
    digitalWrite(IPC_SPI_CS, HIGH);
    delayMicroseconds(1);
    digitalWrite(IPC_SPI_CS, LOW);
    rx[2] = SPI1.transfer16(tx[2]);     // Transfer block 3
    digitalWrite(IPC_SPI_CS, HIGH);
    delayMicroseconds(1);
    digitalWrite(IPC_SPI_CS, LOW);
    rx[3] = SPI1.transfer16(tx[3]);     // Transfer block 4
    digitalWrite(IPC_SPI_CS, HIGH);
    SPI1.endTransaction();

    

    /*Decode received data*/
    decodeData(&rollRX, &pitchRX, rx);

    if(rollRX >= 100600){
      ready_to_sent = true;
    }

    /* Print data in hexadecimal format */
    //////////////////////////////////                                        DATI TRASMESSI                                                ////////////////////////////////////////
    // Print decoded data in decimal format 
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Dati trasmessi decodificati (decimali):");
    DEBUG_PRINT("Roll: ");
    DEBUG_PRINT(roll);  DEBUG_PRINT("\t");
    DEBUG_PRINT("Pitch: ");
    DEBUG_PRINT(pitch);  DEBUG_PRINT("\n");

    /* Print dati trasmessi nel formato spacchettato (2 x 16 bit)
    //DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Dati trasmessi spacchettati (hex):");
    DEBUG_PRINT("Tx_Roll : 0x"); DEBUG_PRINT(tx[1], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(tx[0], HEX); 
    DEBUG_PRINT("Tx_Pitch: 0x"); DEBUG_PRINT(tx[3], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(tx[2], HEX);*/

    //////////////////////////////////                                        DATI RICEVUTI                                                ////////////////////////////////////////
    /* Print dati ricevuti nel formato spacchettato (2 x 16 bit) */
    //DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Dati ricevuti spacchettati (hex):");
    DEBUG_PRINT("Rx_Roll : 0x"); DEBUG_PRINT(rx[1], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(rx[0], HEX);
    DEBUG_PRINT("RX_Pitch: 0x"); DEBUG_PRINT(rx[3], HEX); DEBUG_PRINT(" 0x"); DEBUG_PRINTLN(rx[2], HEX);
    

    // Print dati ricevuti e decodificati in formato esadecimale, con padding a 8 cifre
    //DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Dati ricevuti ricostruiti:");
    char buffer[9];  // Buffer per contenere la stringa esadecimale con terminatore nullo
    sprintf(buffer, "%08X", (uint32_t)rollRX);
    DEBUG_PRINT("Roll (hex): 0x"); DEBUG_PRINTLN(buffer); //DEBUG_PRINT("\t");
    sprintf(buffer, "%08X", (uint32_t)pitchRX);
    DEBUG_PRINT("Pitch (hex): 0x"); DEBUG_PRINT(buffer); DEBUG_PRINT("\n");

    /* Print dati ricevuti decodificati in formato decimale */
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("Dati ricevuti decodificati (decimali):");
    DEBUG_PRINT("Roll (dec): ");
    DEBUG_PRINT(rollRX);  
    DEBUG_PRINT("\t");
    DEBUG_PRINT("Pitch (dec): ");
    DEBUG_PRINT(pitchRX);  
    DEBUG_PRINT("\n");

    /*Debug Prints
     DEBUG_PRINT((float)rollRX/10000);  DEBUG_PRINT("\t");
     DEBUG_PRINT((float)pitchRX/10000);  DEBUG_PRINT("\n");

    //Uncomment to print the single received words
    // DEBUG_PRINT(rx[0]);  DEBUG_PRINT("\t");
    // DEBUG_PRINT(rx[1]);  DEBUG_PRINT("\t");
    // DEBUG_PRINT(rx[2]);  DEBUG_PRINT("\t");
    // DEBUG_PRINT(rx[3]);  DEBUG_PRINT("\n");*/
#endif

    /* Counter Update */
    counter++;
  }

}

void prepareData(int counter, uint16_t* txData, int32_t* add1, int32_t* add2) { // versione per ottenere la stampa dei dati trasmessi in formato decimale

  if(!ready_to_sent){

    *add1 = 0;
    *add2 = 0;

  }else{

    *add1 = 100000 + counter;
    *add2 = 200000 + counter;

  }
  
  txData[0] = (int16_t) (0x8000 | (roll & 0xFFF));                  // ROLL LSB -> txData[0] = 0x8XXX dove: 8 (in binario 1000) sarebbe l'identificatore e XXX sarebbe il payload
  txData[1] = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));       // ROLL MSB -> txData[1] = 0xCXXX dove: C (in binario 1100) sarebbe l'identificatore e XXX sarebbe il payload

  txData[2] = (int16_t) (0x0000 | (pitch & 0xFFF));                 // PITCH LSB -> txData[2] = 0x0XXX dove: 0 (in binario 0000) sarebbe l'identificatore e XXX sarebbe il payload
  txData[3] = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));      // PITCH MSB -> txData[3] = 0x4XXX dove: 4 (in binario 0100)sarebbe l'identificatore e XXX sarebbe il payload

}

/*void prepareData(int counter, uint16_t* txData) {

  int32_t roll = 100000 + counter;
  int32_t pitch = 200000 + counter;

  txData[0] = (int16_t) (0x8000 | (roll & 0xFFF));                  // ROLL LSB -> txData[0] = 0x8XXX dove: 8 (in binario 1000) sarebbe l'identificatore e XXX sarebbe il payload
  txData[1] = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));       // ROLL MSB -> txData[1] = 0xCXXX dove: C (in binario 1100) sarebbe l'identificatore e XXX sarebbe il payload

  txData[2] = (int16_t) (0x0000 | (pitch & 0xFFF));                 // PITCH LSB -> txData[2] = 0x0XXX dove: 0 (in binario 0000) sarebbe l'identificatore e XXX sarebbe il payload
  txData[3] = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));      // PITCH MSB -> txData[3] = 0x4XXX dove: 4 (in binario 0100)sarebbe l'identificatore e XXX sarebbe il payload

}*/

void decodeData(int32_t* data1, int32_t* data2, uint16_t* rxData) {

  *data1 = 0;
  *data2 = 0;

  *data1 |= (0xFFF & rxData[0]);                //Keep only the received 12 lsb (payload) and set them as bits from 0 to 11
  *data1 |= (0xFFF & rxData[1]) << 12;          //Keep only the received 12 lsb (payload) and set them as bits from 12 to 23

  *data2 |= (0xFFF & rxData[2]);                //Keep only the received 12 lsb (payload) and set them as bits from 0 to 11
  *data2 |= (0xFFF & rxData[3]) << 12;

  if (*data1 & 0x800000)                          //If bit 23 of updated data is 1    (Negative values)
    *data1 |= 0xFF000000;                         //Set to 1 bits from 23 to 31
  else
    *data1 &= 0x00FFFFFF;                         //Set to 0 bits from 23 to 31
  if (*data2 & 0x800000)                          //If bit 23 of updated data is 1    
    *data2 |= 0xFF000000;                         //Set to 1 bits from 23 to 31
  else
    *data2 &= 0x00FFFFFF;                         //Set to 0 bits from 23 to 31

}

