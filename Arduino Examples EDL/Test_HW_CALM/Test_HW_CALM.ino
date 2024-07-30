// works with a Feratherboard M0 configuration

#define LED_ON_OFF_SWITCH     13
#define LED_USER_RED          14
#define LED_USER_GREEN        42
#define LED_OK                6
#define LED_FAULT             7

#define BUZZER                32
#define SC_A                  19
#define SC_B                  25
#define KICK_WD_SAMD          16
#define nENUSBV               8
#define NINA_RST              46
#define SAMD_READY            3
#define RM_READY              4

#define IPC_UART_TX           30
#define IPC_UART_RX           31

#define IPC_SPI_SCK           37
#define IPC_SPI_CS            36
#define IPC_SPI_MISO          34
#define IPC_SPI_MOSI          35

// #define BYPASS_BUZZER_TEST

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
  // Configure OnOFF switch LED control pin
  pinMode(LED_ON_OFF_SWITCH,      OUTPUT);
  pinMode(LED_USER_RED,           OUTPUT);
  pinMode(LED_USER_GREEN,         OUTPUT);
  
  pinMode(LED_OK,                 OUTPUT);
  pinMode(LED_FAULT,              OUTPUT);
  pinMode(BUZZER,                 OUTPUT);
  pinMode(SC_A,                   OUTPUT);
  pinMode(SC_B,                   OUTPUT);
  pinMode(KICK_WD_SAMD,           OUTPUT);
  pinMode(nENUSBV,                OUTPUT);
  pinMode(NINA_RST,               OUTPUT);

  pinMode(SAMD_READY,             OUTPUT);
  pinMode(RM_READY,               INPUT);
  pinMode(IPC_UART_TX,            OUTPUT);
  pinMode(IPC_UART_RX,            INPUT);
  pinMode(IPC_SPI_SCK,            OUTPUT);
  pinMode(IPC_SPI_CS,             OUTPUT);
  pinMode(IPC_SPI_MOSI,           OUTPUT);
  pinMode(IPC_SPI_MISO,           INPUT);

  digitalWrite(LED_ON_OFF_SWITCH, LOW);
  digitalWrite(LED_USER_RED,      LOW);
  digitalWrite(LED_USER_GREEN,    LOW);

  digitalWrite(LED_OK,            LOW);
  digitalWrite(LED_FAULT,         LOW);
  digitalWrite(BUZZER,            LOW);
  digitalWrite(SC_A,              HIGH);
  digitalWrite(SC_B,              HIGH);
  digitalWrite(KICK_WD_SAMD,      LOW);
  digitalWrite(nENUSBV,           HIGH);
  digitalWrite(NINA_RST,          HIGH);
  digitalWrite(IPC_UART_TX,       LOW);

  Serial.begin(115200);
}

void loop() {
  int i;
  
  Serial.println("Start testing....");

#ifndef BYPASS_BUZZER_TEST
  Serial.println("Buzzer:");
  for(i=1;i<=2;i++) {
    SLMATIM();
    delay(50);
  }
#endif
  
  Serial.println("LED Switch:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_ON_OFF_SWITCH, HIGH);
    delay(100);
    
    digitalWrite(LED_ON_OFF_SWITCH, LOW); 
    delay(100);
  }

  Serial.println("LED User RED:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_USER_RED, HIGH);
    delay(100);
    
    digitalWrite(LED_USER_RED, LOW); 
    delay(100);
  }

  Serial.println("LED User GREEN:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_USER_GREEN, HIGH);
    delay(100);
    
    digitalWrite(LED_USER_GREEN, LOW); 
    delay(100);
  }

  Serial.println("LED Ok:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_OK, HIGH);  
    delay(100);
    digitalWrite(LED_OK, LOW);
    delay(100);
  }

  Serial.println("LED Fault:");
  for(i=1;i<=10;i++) {
    digitalWrite(LED_FAULT, HIGH);  
    delay(100);
    digitalWrite(LED_FAULT, LOW);
    delay(100);
  }

  Serial.println("SC_A:");
  for(i=1;i<=10;i++) {
    digitalWrite(SC_A, HIGH);  
    delay(100);
    digitalWrite(SC_A, LOW);
    delay(100);
  }
  
  Serial.println("SC_B:");
  for(i=1;i<=10;i++) {
    digitalWrite(SC_B, HIGH);  
    delay(100);
    digitalWrite(SC_B, LOW);
    delay(100);
  }

  Serial.println("KICK_WD_SAMD:");
  for(i=1;i<=10;i++) {
    digitalWrite(KICK_WD_SAMD, HIGH);  
    delay(100);
    digitalWrite(KICK_WD_SAMD, LOW);
    delay(100);
  }

  Serial.println("nENUSBV:");
  for(i=1;i<=10;i++) {
    digitalWrite(nENUSBV, LOW);  
    delay(100);
    digitalWrite(nENUSBV, HIGH);
    delay(100);
  }

  Serial.println("Test Completed, waiting for RM...");
  digitalWrite(SAMD_READY, HIGH);
  delay(1000);
  digitalWrite(SAMD_READY, LOW);

  bool rm_ready = false;
  for(int i = 0; i < 100; ++i)
  {
    if(digitalRead(RM_READY) == HIGH)
    {
      Serial.println("RM_READY");
      rm_ready = true;
      break;
    }
    
    delay(100);
  }

  if(!rm_ready)
  {
    Serial.println("No news from RM");
    return;
  }

  // IPC Lines test
  Serial.println("Testing IPC Lines");
  
  if(digitalRead(IPC_UART_RX) == HIGH)
  {
    Serial.println("UART line unexpectedly HIGH");
  }
  else
  {
    digitalWrite(IPC_UART_TX, HIGH);
    delay(300);
    digitalWrite(IPC_UART_TX, LOW);

    bool ping_ok = false;
    for(int i = 0; i < 10; ++i)
    {
      if(digitalRead(IPC_UART_RX) == HIGH)
      {
        Serial.println("IPC UART ping OK");
        ping_ok = true;
        break;
      }
      delay(50);
    }

    if(!ping_ok)
    {
      Serial.println("IPC UART ping unanswered!!");
    }
  }
  
  if(digitalRead(IPC_SPI_MISO) == HIGH)
  {
    Serial.println("SPI line unexpectedly HIGH");
  }
  else
  {
    digitalWrite(IPC_SPI_SCK, HIGH);
    digitalWrite(IPC_SPI_CS, HIGH);
    digitalWrite(IPC_SPI_MOSI, HIGH);
    delay(300);
    digitalWrite(IPC_SPI_SCK, LOW);
    digitalWrite(IPC_SPI_CS, LOW);
    digitalWrite(IPC_SPI_MOSI, LOW);

    bool ping_ok = false;
    for(int i = 0; i < 10; ++i)
    {
      if(digitalRead(IPC_SPI_MISO) == HIGH)
      {
        Serial.println("IPC SPI ping OK");
        ping_ok = true;
        break;
      }
      delay(50);
    }

    if(!ping_ok)
    {
      Serial.println("IPC SPI ping unanswered!!");
    }
  }
}
