#include "calm_pins.h"
//#include "trajectory.h"

// Require mouse control library
#include <MouseController.h>

#define LED_OFF_DELAY_ms 20           // Delay in ms to turn off the led after mouse motion stops
#define SCALING_MOUSE 4               // Scale factor for mouse x and y data

/******************************************************************/

unsigned long last_time_moved = 0;

/* USB */
USBHost usb;                          // Initialize USB Controller
MouseController mouse(usb);           // Attach mouse controller to USB

int32_t position_x = 0;
int32_t position_y = 0;
uint8_t motion = 0;


/* This function intercepts mouse motion */
void mouseMoved()
{
  position_x -= SCALING_MOUSE * mouse.getXChange();
  position_y -= SCALING_MOUSE * mouse.getYChange();

  motion = 1;
}

void setup() {

  pinMode(LED_FAULT,      OUTPUT);
  pinMode(LED_OK,         OUTPUT);
  pinMode(nENUSBV,        OUTPUT);

  digitalWrite(nENUSBV, LOW);                     // Enable USB power (Active low)


  /* Led visual feedback for program start */
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_OK, HIGH);
    delay(100);
    digitalWrite(LED_OK, LOW);
    delay(100);
  }

}

void loop() {
  
  usb.Task();                                     // Process USB tasks

  /* Led control over mouse motion */
  if (motion == 1)
    {
      digitalWrite(LED_OK, HIGH);
      last_time_moved = millis();
    }
    else
    {
      if (millis() >= last_time_moved + LED_OFF_DELAY_ms)
      {
        digitalWrite(LED_OK, LOW);
      }      
    }
    motion = 0;
 
}
