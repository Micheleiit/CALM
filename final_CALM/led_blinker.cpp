/*
* led_blinker.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "led_blinker.h"

// Inizializza LedBlinker
void initLedBlinkerStruct(LedBlinkerStruct* ledBlinkerStruct, int pin, unsigned long interval){
  ledBlinkerStruct->pin = pin;
  ledBlinkerStruct->interval = interval;
  ledBlinkerStruct->lastTime = 0;
  ledBlinkerStruct->isOn = false;

  pinMode(ledBlinkerStruct->pin, OUTPUT);
  digitalWrite(ledBlinkerStruct->pin, LOW);
}

// Aggiorna lo stato del LED in base all'intervallo di lampeggio
void updateLedBlinkerStruct(LedBlinkerStruct* ledBlinkerStruct){
  unsigned long currentTime = millis(); // Ottieni il tempo attuale

  if(currentTime - ledBlinkerStruct->lastTime >= ledBlinkerStruct->interval){
    ledBlinkerStruct->lastTime = currentTime;

    // cambia lo stato del LED
    if(ledBlinkerStruct->isOn){

      digitalWrite(ledBlinkerStruct->pin, LOW);
      ledBlinkerStruct->isOn = false;

    } else {

      digitalWrite(ledBlinkerStruct->pin, HIGH);
      ledBlinkerStruct->isOn = true;

    }
  }
}