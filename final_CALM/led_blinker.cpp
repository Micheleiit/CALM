/*
* led_blinker.cpp
*
* Created on: August, 2024
*   Author: Michele Di Lucchio
*    
*/

#include "led_blinker.h"

// Inizializza LedBlinker
void initLedBlinkerStruct(LedBlinkerStruct* ledBlinkerStruct, int pin){
  ledBlinkerStruct->pin = pin;
  ledBlinkerStruct->lastTime = 0;
  ledBlinkerStruct->isOn = false;

  pinMode(ledBlinkerStruct->pin, OUTPUT);
  digitalWrite(ledBlinkerStruct->pin, LOW);
}

// Aggiorna lo stato del LED in base all'intervallo di lampeggio
void enableLedBlink(LedBlinkerStruct* ledBlinkerStruct, unsigned long period){
  unsigned long currentTime = millis(); // Ottieni il tempo attuale

  if(currentTime - ledBlinkerStruct->lastTime >= period){
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

void errorSignal(LedBlinkerStruct* ledBlinkerStruct1, LedBlinkerStruct* ledBlinkerStruct2, unsigned long period){
  unsigned long currentTime = millis(); // Ottieni il tempo attuale una sola volta

  // Aggiorna lo stato di entrambi i LED
  if(currentTime - ledBlinkerStruct1->lastTime >= period) {

    // Cambia lo stato del primo LED
    ledBlinkerStruct1->lastTime = currentTime;
    ledBlinkerStruct1->isOn = !ledBlinkerStruct1->isOn;
    digitalWrite(ledBlinkerStruct1->pin, ledBlinkerStruct1->isOn ? HIGH : LOW);

    // Cambia lo stato del secondo LED
    ledBlinkerStruct2->lastTime = currentTime;
    ledBlinkerStruct2->isOn = !ledBlinkerStruct2->isOn;
    digitalWrite(ledBlinkerStruct2->pin, ledBlinkerStruct2->isOn ? HIGH : LOW);

  }
}