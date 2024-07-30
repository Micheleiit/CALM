/*
  LED

  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <CALMBLE.h>
#include "pin_map.h"


// I servizi sono identificati da numeri univoci noti come UUID
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); //  Definisce un servizio BLE per il LED (ledService) con un UUID specifico

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central.  I servizi standard hanno un UUID a 16 bit e i servizi personalizzati hanno un UUID a 128 bit.
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // definisce una caratteristica switchCharacteristic che rappresenta l'interruttore del LED. Questa caratteristica è leggibile e scrivibile da un dispositivo centrale BLE

const int ledPin =  LED_FAULT; // pin to use for the LED

void setup() {
  //pinMode(LED_OK, OUTPUT);
  //pinMode(NINA_RST, OUTPUT);
  //digitalWrite(NINA_RST, LOW);
  //digitalWrite(NINA_RST, HIGH);
  pinMode(LED_OK, OUTPUT);
  digitalWrite(LED_OK, HIGH);

  Serial.begin(9600);
  while (!Serial){
    delay(100);
    Serial.println("Waiting for Serial connection....");

  };

  //Serial.println("Hello there!");

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);
  
  //pinMode(3, OUTPUT);
  //digitalWrite(3, LOW);

  // Numero massimo di tentativi di inizializzazione del BLE
  const int maxAttempts = 10;
  int attempt = 0;
  bool bleInitialized = false;

  // Tentativi di inizializzazione del BLE
  while (attempt < maxAttempts && !bleInitialized) {
    Serial.print("Attempting to start BLE, attempt ");
    Serial.println(attempt + 1);

    if (BLE.begin()) {
      bleInitialized = true;
    } else {
      Serial.println("starting BLE failed!");
      attempt++;
      delay(1000); // Attendere un secondo prima di ritentare
    }
  }

  if (!bleInitialized) {
    Serial.println("Failed to start BLE after multiple attempts, entering infinite loop.");
    while (1);
  } else {
    Serial.println("BLE ok!");
  }

  /* begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }*/

  /* set advertised local name and service UUID: */
  BLE.setLocalName("LED"); // Imposta il nome locale del dispositivo BLE.
  BLE.setAdvertisedService(ledService); // Imposta il servizio che verrà pubblicizzato.

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic); // Aggiunge la caratteristica al servizio

  // add service
  BLE.addService(ledService); // Aggiunge il servizio BLE.

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0); // Imposta il valore iniziale della caratteristica a 0 (LED spento).

  // start advertising
  BLE.advertise(); //  Inizia a pubblicizzare il dispositivo BLE

  Serial.println("BLE LED Peripheral"); // Stampa un messaggio indicando che il dispositivo BLE è pronto.

  

}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central(); //  Ascolta le connessioni dai dispositivi centrali BLE.

  // if a central is connected to peripheral:
  if (central) { // e un dispositivo centrale si connette:
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) { //  Se il dispositivo centrale ha scritto un valore nella caratteristica:
        if (switchCharacteristic.value()) {   // se il valore scritto è diverso da 0
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // will turn the LED on
        } else {                              // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);          // will turn the LED off
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
