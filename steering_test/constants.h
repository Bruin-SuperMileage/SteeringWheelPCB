#ifndef CONST_H
#define CONST_H

// INPUTS //
#define DAQ_BUTTON 14
#define HEADLIGHT_SWITCH 15
#define LEFT_TURN_SWITCH 16
#define RIGHT_TURN_SWITCH 17
#define HAZARD_SWITCH 18
#define BRAKE_SWITCH 19
#define HORN_SWITCH 20
#define WIPER_SWITCH 21
#define LIMIT_SWITCH 13

// OUTPUTS //
#define HEADLIGHT_MOSFET 0
#define BRAKE_MOSFET 4
#define HORN_MOSFET 5
#define LED1 7
#define MASTER_TURN_MOSFET 8
#define IN_1 9
#define IN_2 10
#define IN_3 11
#define IN_4 12

// --------------- LED GLOBAL ---------------
enum LED_STATE {ON, OFF, ON_DELAY, OFF_DELAY};
double brightness = 1.0;

// --------------- BRAKE LED CONTROL ---------------
const int BRAKE_NUM_LEDS = 11;
CRGB brake_leds[BRAKE_NUM_LEDS];

// --------------- TURN LED CONTROL ---------------
#define FRONT_LEFT_TURN_LED_PIN         1
#define FRONT_RIGHT_TURN_LED_PIN        2
#define BACK_LEFT_TURN_LED_PIN          3
#define BACK_RIGHT_TURN_LED_PIN         6
#define BRAKE_LED_PIN 4
const int TURN_NUM_LEDS               = 11;
const int TURN_SHOW_DELAY             = 50;
const int TURN_TRANSITION_DELAY       = 500;

// Container class for our variables
struct LEDStateStrip {
  CRGB leds[TURN_NUM_LEDS];
  int leds_on;
  int leds_off;
  long unsigned int current_time;
  LED_STATE state;
};

// --------------- WIPER CONTROL ----------------
enum WIPER_STATE {EXTEND, RETRACT, EXTEND_DELAY, RETRACT_DELAY};
enum RESET_STATE {START_DELAY, HOMING, END_DELAY, RESET};

struct WiperController {
  const long unsigned int mosfet_delay = 2000;
  long unsigned int current_time;
  bool home;
  WIPER_STATE state;
  RESET_STATE reset_state;
  long unsigned int return_time;
  bool flag;
};

#endif
