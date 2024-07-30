
/*
  Interface controller for CALM system, runbning on an Arduino MKRZERO.

  Reads a predefined trajectory, stored in an .h file

  Flow:

  - Export a vectorial drawing in gcode (.ngc) in inkscape
  - Process it with parse_gcode.m
  - Include the generated .h file in this project
  - Change the variable traj->current_size to the lenght of the trajectory -1.
*/

// Require mouse control library
#include <MouseController.h>
#include <Usb.h>
#include <SPI.h>
#include "trajectory.h"

// Chip select pin for spi communication
#define CS_PIN 1

/* Time interval between each reference communication to the manipulator, in ms.
    Note that this is not a hard scheduling. If the calculations take more time than this period, the spi communication is delayed and the movement is executed slowlier.
    ==> Execution of debug "Serial.print()" WILL slow the trajectory down.
*/
#define STREAMING_PERIOD_ms 1

#define SCALING_MOUSE 4

// Initialize USB Controller
USBHost usb;

// Attach mouse controller to USB
MouseController mouse(usb);

// variables for mouse button states
boolean leftButton = false;
boolean middleButton = false;
boolean rightButton = false;

// Execution states of the finite state machine
enum states {FREE_HAND, RECORDING, DRAW_RECORD, DRAW_LINE, DRAW_TAIL} current_state;

// Pen position
int32_t position_x = 0;
int32_t position_y = 0;

// Position for line scanning
int32_t position_line_x = 0;
int32_t position_line_y = 0;

// Position for tail scanning
int32_t position_tail_x = 0;
int32_t position_tail_y = 0;

// For line scanning
float angle_line = 0;
int scale_line;
bool line_rotation;

//Accumulator for dstinction between short press and long press
uint32_t right_press_time = 0;

//For scheduling. //!\\ Will overflow after approx 50days
unsigned long millis_prev = 0;

trajectory_calm* traj_record;

/*
   Saturation function (Symmetrical)
*/
int32_t saturationInteger(int32_t boundary, int32_t value)
{
  if (value < boundary)
  {
    if (value > -boundary)
      return value;
    else
      return -boundary;
  }
  else
    return boundary;
}

/*
   Send a command to the manipulator, by SPI.

   The SPI communication is over 16bits data words.
   However, the encoder references need more than that. In consequence, each reference is divided into two 12 bits number.
   To these 12 bits is added an adress over 4 bits.
*/
void send_to_manipulator(int32_t x, int32_t y)
{
  int32_t roll = x;
  int32_t pitch = y / 2;


  /*
     Adresses:
     - 1000: Roll, low bytes
     - 1100: Roll, high bytes
     - 0000: Pitch, low bytes
     - 0100: Pitch, high bytes
  */

  /*
     Note: This connection is not in the secure execution zone: Security measures may be implemented on the manipulator controller for handling errors:
     - Limiting the speed (done)
     - Limiting the workspace (done)
     - Checking the two lower bytes of the adresses.

     However, it may be interesting to implement a write-read routine for verification, and/or to implement a parity check using the two unused bits.
  */
  int16_t roll_l = (int16_t) (0x8000 | (roll & 0xFFF));
  int16_t roll_h = (int16_t) (0xC000 | ((roll & 0xFFF000) >> 12));

  int16_t pitch_l = (int16_t) (0x0000 | (pitch & 0xFFF));
  int16_t pitch_h = (int16_t) (0x4000 | ((pitch & 0xFFF000) >> 12));

  noInterrupts(); // Not sure, may affect the USB communication
  digitalWrite(CS_PIN, LOW);
  SPI.transfer16(roll_l);
  digitalWrite(CS_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer16(roll_h);
  digitalWrite(CS_PIN, HIGH);

  delayMicroseconds(1);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer16(pitch_l);
  digitalWrite(CS_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer16(pitch_h);
  digitalWrite(CS_PIN, HIGH);

  interrupts();


}

// This function intercepts mouse movements
void mouseMoved() {

  position_x -= SCALING_MOUSE * mouse.getXChange();
  position_y -= SCALING_MOUSE * mouse.getYChange();

  position_x = saturationInteger(30000, position_x);
  position_y = saturationInteger(30000, position_y);


  if (current_state == FREE_HAND)
  {
    // In this case, the position is send only when there is an input from the mouse, not at 1kHz.
    // The low pass filter on the controller is this important for smoothing.
    send_to_manipulator(position_x, position_y);
  }
}

void mouseDragged()
{

  mouseMoved(); // mouseMoved is not called automatically if one button is pressed.
}


// This function intercepts mouse button press
void mousePressed() {
  if (mouse.getButton(LEFT_BUTTON) && !leftButton)
  {
    current_state = DRAW_RECORD;
    traj_record->current_index = 0;
    traj_record->current_direction = 1;
    middleButton = true;
  }
  if (mouse.getButton(MIDDLE_BUTTON) && !middleButton)
  {
    current_state = FREE_HAND;
    traj_record->current_index = 0;

    middleButton = true;
  }
  if (mouse.getButton(RIGHT_BUTTON) && !rightButton)
  {
    current_state = DRAW_LINE;
    rightButton = true;
  }

}

// This function intercepts mouse button release
void mouseReleased() {
  if ( !mouse.getButton(LEFT_BUTTON) && leftButton) {

    leftButton = false;
  }
  if (!mouse.getButton(MIDDLE_BUTTON) && middleButton) {

    middleButton = false;
  }
  if (!mouse.getButton(RIGHT_BUTTON) && rightButton) {
    current_state = FREE_HAND;
    rightButton = false;

  }
}


void setup() {

  // start the SPI library:
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1)); // Hercules: Leave the SPI Data Formats as is.
  pinMode(CS_PIN, OUTPUT);

  Serial1.begin(115200);

  traj_record = init_trajectory_struct();

  Serial1.println("Program started");
  delay(200);

  send_to_manipulator(0, 0);
  current_state = DRAW_RECORD;
}

void loop() {
  // Process USB tasks
  usb.Task();

  boolean abort_sequence = false;

  if (millis_prev + STREAMING_PERIOD_ms  <= millis())
  {

    millis_prev = millis();

    switch (current_state) {
      case DRAW_RECORD:
        abort_sequence = !read_trajectory_var_speed(traj_record, &position_x, &position_y, 10000);
        send_to_manipulator(position_x, position_y);
        break;
      case DRAW_LINE:
        position_line_x = 10000 * sin(millis() / 500.0) + position_x;
        position_line_y = position_y;
        send_to_manipulator(position_line_x, position_line_y);
        break;

    }
    if (abort_sequence)
    {
      current_state = FREE_HAND;
    }

  }
}
