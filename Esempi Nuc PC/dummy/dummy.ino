#include <SPI.h>

int i = 0;
void setup() {
  // start the SPI library:
  //SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE2));

  // start the SPI library:
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  Serial.begin(115200);
}

void loop() {



  int32_t roll = (int32_t) 2000*sin(2*M_PI*5*(millis()/1000.0));
  int32_t pitch = (int32_t) 2000*cos(2*M_PI*5*(millis()/1000.0))/2.0;

  int16_t roll_l = (int16_t) (0x8000 | (roll & 0xFFF));
  int16_t roll_h = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));

  int16_t pitch_l = (int16_t) (0x0000 | (pitch & 0xFFF));
  int16_t pitch_h = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));

  SPI.transfer16(roll_l);
  delayMicroseconds(10);
  SPI.transfer16(roll_h);

delayMicroseconds(10);
  SPI.transfer16(pitch_l);
  delayMicroseconds(10);
  SPI.transfer16(pitch_h);

Serial.print(roll);
Serial.print(' ');
Serial.println(pitch);
  delay(10);
  i += 1;
}
