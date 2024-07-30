// I pin sono mappati secondo il file variant.cpp
//#define LED_BUILTIN 13 
#define LED_RED 14
#define LED_GREEN 42
#define EnUSB 8 // questo pin non sembra esssere mappato correttamente. Ho visto che era il pin 8 dall'esempio 03 del SAMD

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED_13, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(EnUSB, OUTPUT);
  digitalWrite(EnUSB, LOW);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(EnUSB, LOW);
  Serial.println("Blink LEDOOS...");
  for(int i = 0; i<10; i++){
    digitalWrite(PIN_LED_13, HIGH);
    delay(100);
    digitalWrite(PIN_LED_13, LOW);
    delay(100);
  }

  Serial.println("Blink LED_RED...");
  for(int i = 0; i<10; i++){
    digitalWrite(LED_RED, HIGH);
    delay(100);
    digitalWrite(LED_RED, LOW);
    delay(100);
  }

  Serial.println("Blink LED_GREEN...");
  for(int i = 0; i<10; i++){
    digitalWrite(LED_GREEN, HIGH);
    delay(100);
    digitalWrite(LED_GREEN, LOW);
    delay(100);
  }

}
