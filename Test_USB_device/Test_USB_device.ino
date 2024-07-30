/*
 Mouse Controller Example

 Shows the output of a USB Mouse connected to 
 the Native USB port.

 http://arduino.cc/en/Tutorial/MouseController

 NOTE: sistemare la parte relativa al DRAG perchè non funziona!!!!!!!!!!

 */

// Require mouse control library
#include "calm_pins.h"
#include <MouseController.h>

#define LED_OFF_DELAY_ms 20  // La durata (in millisecondi) dopo la quale il LED viene spento se non viene rilevato alcun movimento.

/******************************************************************/

unsigned long last_time_moved = 0; // Una variabile che memorizza il tempo (in millisecondi) in cui è stato rilevato l'ultimo movimento del mouse

// Initialize USB Controller
USBHost usb; // USBHost is the base class for all calls that rely on USB host communication. When invoked, it initializes a USB controller

// Attach mouse controller to USB
MouseController mouse(usb); // MouseController is the class for all calls to the USBHost relating to an attached mouse

// variables for mouse button states
boolean leftButton = false;
boolean middleButton = false;
boolean rightButton = false;

// variables for mouse movement state
uint8_t motion = 0;
int dispX = 0;
int dispY = 0;
int dispX_Drag = 0;
int dispY_Drag = 0;
boolean drag = false;

// This function intercepts mouse movements and it's called whenever a connected USB mouse moves
void mouseMoved() {  

  motion = 1;

  int xChange = mouse.getXChange();  // mouse.getXChange() Riporta la quantità relativa di movimento del mouse sull'asse X dall'ultimo polling. Un numero positivo indica un movimento verso destra, un numero negativo indica un movimento verso sinistra.
  dispX = xChange;
  int yChange = mouse.getYChange();  // mouse.getYChange() Riporta la quantità relativa di movimento del mouse sull'asse Y dall'ultimo polling. Un numero positivo indica un movimento verso il basso, un numero negativo indica un movimento verso l'alto.
  dispY = yChange;

}

// This function is called whenever a connected USB mouse is dragged (movement while a mouse button is being pressed)
void mouseDragged() {

  drag = true;

  int xChange_Drag = mouse.getXChange();
  dispX_Drag = xChange_Drag;  
  int yChange_Drag = mouse.getYChange();
  dispY_Drag = yChange_Drag;

}

// This function is called whenever a button on a connected USB mouse is pressed.
void mousePressed() {

  
  if (mouse.getButton(LEFT_BUTTON)){

    leftButton = true;
    digitalWrite(LED_USER_RED, HIGH);

  }

  if (mouse.getButton(MIDDLE_BUTTON)){

    middleButton = true;
    digitalWrite(LED_USER_GREEN, HIGH);

  }

  if (mouse.getButton(RIGHT_BUTTON)){

    rightButton = true;
    digitalWrite(BUZZER ,HIGH);

  }
}

// This function intercepts mouse button release
void mouseReleased() {

  if (!mouse.getButton(LEFT_BUTTON) && leftButton==true) {

    leftButton = false;
    digitalWrite(LED_USER_RED, LOW);

  }

  if (!mouse.getButton(MIDDLE_BUTTON) && middleButton==true) {

    middleButton = false;
    digitalWrite(LED_USER_GREEN, LOW);

  }

  if (!mouse.getButton(RIGHT_BUTTON) && rightButton==true) {

    rightButton = false;
    digitalWrite(BUZZER, LOW);

  }

}

void setup()
{

  pinMode(LED_ON_OFF_SWITCH, OUTPUT);
  pinMode(LED_USER_RED, OUTPUT);
  pinMode(LED_USER_GREEN, OUTPUT);
  pinMode(LED_OK,  OUTPUT);
  pinMode(LED_FAULT, OUTPUT);
  pinMode(nENUSBV, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_ON_OFF_SWITCH, LOW);
  digitalWrite(LED_USER_RED, LOW);
  digitalWrite(LED_USER_GREEN, LOW);
  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_FAULT, LOW);
  digitalWrite(nENUSBV, LOW); // Enable USB power (Active low)
  digitalWrite(BUZZER, LOW);

  /* Led visual feedback for program start */
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_USER_GREEN, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(LED_USER_GREEN, LOW);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

void loop()
{
  
  dispX = 0;
  dispY = 0;
  
  usb.Task(); // Process USB tasks:  polls connected usb devices for updates to their status
  
  // Led control over mouse motion 
  if (motion == 1) // è stato rilevato un movimento
  {

    if(dispX != 0){ // il movimento è lungo X

        digitalWrite(LED_OK, HIGH);
        last_time_moved = millis(); /* Viene aggiornato last_time_moved con il tempo corrente (millis()), che rappresenta l'ultima volta in cui è stato rilevato un movimento del mouse. 
        Notare che usare millis() per gestire il tempo è preferibile rispetto a delay() poichè delay() blocca l'esecuzione del codice mentre millis() consente di eseguire altre operazioni simultanemanete */     
    }

    if(dispY != 0){
        digitalWrite(LED_FAULT, HIGH);
        last_time_moved = millis();
    }

    if(drag){
        digitalWrite(LED_ON_OFF_SWITCH, HIGH);
        last_time_moved = millis();
    }
      
  }
    else
    {
      if (millis() >= last_time_moved + LED_OFF_DELAY_ms) // se è passato un periodo di tempo (LED_OFF_DELAY_ms) dall'ultima volta in cui è stato rilevato un movimento.
      {
        digitalWrite(LED_OK, LOW);
        digitalWrite(LED_FAULT, LOW);
        digitalWrite(LED_ON_OFF_SWITCH, LOW);
      }      
    }
    motion = 0;
    drag = false;
}
