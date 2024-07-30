#define LED_BUILDIN 13 // sarebbe lo USERLED secondo la mappatura variant.cpp

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILDIN, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILDIN, HIGH);
  Serial.println("Acceso");
  delay(10000);
  digitalWrite(LED_BUILDIN, LOW);
  Serial.println("Spento");
  delay(3000);

}
