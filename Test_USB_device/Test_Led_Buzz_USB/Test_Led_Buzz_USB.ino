#include "pin_map.h"
#include <MouseController.h>

//#define BYPASS_BUZZER_TEST

// Initialize USB Controller
USBHost usb;

// Attach mouse controller to USB
MouseController mouse(usb);

// do anoing noises to upset the colegues 
void SLMATIM()
{
  long i;
  
  for (i=0; i<100; i++) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(200);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(200);
  }
}

void setup() {
  
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

  //Serial.begin(9600);

  /* Led visual and audio feedback for program start */
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_ON_OFF_SWITCH, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(LED_ON_OFF_SWITCH, LOW);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

void loop() {

  int i;

  for(i = 1; i<10; i++){
    
    //digitalWrite(LED_ON_OFF_SWITCH, HIGH);
    usb.Task();
    
    delay(1000);

  }
  
  //Serial.println("Start testing....");
  //delay(100);
  digitalWrite(LED_ON_OFF_SWITCH, LOW);

  #ifndef BYPASS_BUZZER_TEST
  //Serial.println("Buzzer:");
  for(i=1;i<=2;i++) {
    SLMATIM();
    delay(50);
  }
  #endif

  /* Per monitorare le periferiche è importamte assicurarsi che il resto del ciclo loop() non impieghi molto tempo altrimenti potrebbero verificarsi dei ritardi nell gestione della cominicazione USB.
  Vedere la soluzione dello sketch Test_USB_Device.
  
  //Serial.println("LED Switch:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_ON_OFF_SWITCH, HIGH);
    delay(100);
    
    digitalWrite(LED_ON_OFF_SWITCH, LOW); 
    delay(100);
  }

  //Serial.println("LED User RED:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_USER_RED, HIGH);
    delay(100);
    
    digitalWrite(LED_USER_RED, LOW); 
    delay(100);
  }

  //Serial.println("LED User GREEN:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_USER_GREEN, HIGH);
    delay(100);
    
    digitalWrite(LED_USER_GREEN, LOW); 
    delay(100);
  }

  //Serial.println("LED Ok:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_OK, HIGH);  
    delay(100);
    digitalWrite(LED_OK, LOW);
    delay(100);
  }

  //Serial.println("LED Fault:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_FAULT, HIGH);  
    delay(100);
    digitalWrite(LED_FAULT, LOW);
    delay(100);
  }

  //Serial.println("nENUSBV:");
  for(i=1;i<=10;i++) {
    digitalWrite(nENUSBV, LOW);  
    delay(1000);
    digitalWrite(nENUSBV, HIGH);
    delay(1000);
  }*/
}
