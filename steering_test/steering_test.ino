#include <FastLED.h>

// INPUTS //

#define DAQ_BUTTON 14
#define HEADLIGHT_SWITCH 15
#define LEFT_TURN_SWITCH 16
#define RIGHT_TURN_SWITCH 17
#define HAZARD_SWITCH 18
#define BRAKE_SWITCH 19 
#define HORN_SWITCH 20
#define WIPER_SWITCH 21

// OUTPUTS //

#define HEADLIGHT_MOSFET 0
#define LEFT_TURN_MOSFET 1
#define RIGHT_TURN_MOSFET 2
#define BRAKE_MOSFET 4
#define HORN_MOSFET 5
#define LED1 7
#define LED2 8
#define IN_1 9
#define IN_2 10
#define IN_3 11
#define IN_4 12

// --------------- LED GLOBAL ---------------

enum LED_STATE {ON, OFF, ON_DELAY, OFF_DELAY};

double brightness = 1.0;

// --------------- BRAKE LED CONTROL ---------------

#define BRAKE_LED_PIN      1
const int BRAKE_NUM_LEDS = 10;

CRGB brake_leds[BRAKE_NUM_LEDS];

// --------------- TURN LED CONTROL ---------------

#define FRONT_LEFT_TURN_LED_PIN         1
#define FRONT_RIGHT_TURN_LED_PIN        2
#define BACK_LEFT_TURN_LED_PIN          3
#define BACK_RIGHT_TURN_LED_PIN         6
const int TURN_NUM_LEDS               = 10;
const int TURN_SHOW_DELAY             = 50;
const int TURN_TRANSITION_DELAY       = 500;

CRGB left_leds[TURN_NUM_LEDS];
CRGB right_leds[TURN_NUM_LEDS];

long unsigned int current_left_turn_time;
long unsigned int current_right_turn_time;

int left_turn_leds_on = 0;
int left_turn_leds_off = 0;
int right_turn_leds_on = 0;
int right_turn_leds_off = 0;

LED_STATE left_turn_led_state = ON;
LED_STATE right_turn_led_state = ON;


// --------------- PRE-SET-UP ---------------

bool read_switch(int port);
bool update_device(int port);

void reset_leds(CRGB leds[], long unsigned int &current_turn_time, LED_STATE &turn_led_state, int &turn_leds_on, int &turn_leds_off);
void set_leds_turn(CRGB leds[], long unsigned int &current_turn_time, LED_STATE &turn_led_state, int &turn_leds_on, int &turn_leds_off);

// ------------------------------------------------------------------------------ SETUP () --------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
 
 // INPUT FUNCTION // 
  pinMode(DAQ_BUTTON, INPUT);
  pinMode(HEADLIGHT_SWITCH, INPUT);
  pinMode(LEFT_TURN_SWITCH, INPUT);
  pinMode(RIGHT_TURN_SWITCH, INPUT);
  pinMode(HAZARD_SWITCH, INPUT);
  pinMode(BRAKE_SWITCH, INPUT);
  pinMode(HORN_SWITCH, INPUT);
  pinMode(WIPER_SWITCH, INPUT);


  // OUTPUTS //
  pinMode(HEADLIGHT_MOSFET, OUTPUT);
  pinMode(LEFT_TURN_MOSFET, OUTPUT);
  pinMode(RIGHT_TURN_MOSFET, OUTPUT);
  pinMode(BRAKE_MOSFET, OUTPUT);
  pinMode(HORN_MOSFET, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  

  // BRAKE LED SET-UP //

  //FastLED.addLeds<WS2812, BRAKE_LED_PIN, GRB>(brake_leds, BRAKE_NUM_LEDS);

  // TURN LED SET-UP //

  FastLED.addLeds<WS2812, FRONT_LEFT_TURN_LED_PIN, GRB>(left_leds, TURN_NUM_LEDS);
  FastLED.addLeds<WS2812, FRONT_RIGHT_TURN_LED_PIN, GRB>(right_leds, TURN_NUM_LEDS);
  
  current_left_turn_time = millis();
  current_right_turn_time = millis();
}


// ------------------------------------------------------------------------ MAIN LOOP ---------------------------------------------------------------------------
void loop() {


  // Turn and turn LED setting

  Serial.print("Hazard Switch: ");
  Serial.print(read_switch(HAZARD_SWITCH));
  Serial.print(", Left Switch: ");
  Serial.print(read_switch(LEFT_TURN_SWITCH));
  Serial.print(", Right Switch: ");
  Serial.println(read_switch(RIGHT_TURN_SWITCH));

  delay(50);

  if(read_switch(HAZARD_SWITCH)){
    set_leds_turn(left_leds, current_left_turn_time, left_turn_led_state, left_turn_leds_on, left_turn_leds_off);
    set_leds_turn(right_leds, current_right_turn_time, right_turn_led_state, right_turn_leds_on, right_turn_leds_off);
  } 
  
  else if(read_switch(LEFT_TURN_SWITCH)){
    reset_leds(right_leds, current_right_turn_time, right_turn_led_state, right_turn_leds_on, right_turn_leds_off);
    set_leds_turn(left_leds, current_left_turn_time, left_turn_led_state, left_turn_leds_on, left_turn_leds_off);
  } 
  
  else if(read_switch(RIGHT_TURN_SWITCH)){
    reset_leds(left_leds, current_left_turn_time, left_turn_led_state, left_turn_leds_on, left_turn_leds_off);
    set_leds_turn(right_leds, current_right_turn_time, right_turn_led_state, right_turn_leds_on, right_turn_leds_off);
  } 
  
  else {
    reset_leds(left_leds, current_left_turn_time, left_turn_led_state, left_turn_leds_on, left_turn_leds_off);
    reset_leds(right_leds, current_right_turn_time, right_turn_led_state, right_turn_leds_on, right_turn_leds_off);
  }
  
  
  // End of brake LED stuff
}

// ------------------------------------------------------------------------ END OF MAIN LOOP ---------------------------------------------------------------------------


// SWITCH & OUTPUT MANAGING FUNCTIONS

bool read_switch(int port){
  return digitalRead(port);
}


bool update_device(int input_port, int output_port){
  bool state = read_switch(input_port);

  digitalWrite(output_port, state);
  return state;
}

// LED TURN STATE MACHINE

void set_leds_turn(CRGB leds[], long unsigned int &current_turn_time, LED_STATE &turn_led_state, int &turn_leds_on, int &turn_leds_off){

  // ------------------------------------------- TURNING TURN LEDS ON -------------------------------------------
  if(turn_led_state == ON) {

    if(turn_leds_on < TURN_NUM_LEDS) {
      
      if(millis() - current_turn_time > TURN_SHOW_DELAY){

        leds[turn_leds_on].setRGB((int)(255 * brightness), 0, 0);
        FastLED.show();
        turn_leds_on++;
        current_turn_time = millis();
      }
      
    } else{
      turn_led_state = ON_DELAY;
      turn_leds_on = 0;
      current_turn_time = millis();
      FastLED.show();
    } 

  // ------------------------------------------- TURN LEDS ON, DELAYING -------------------------------------------
  } else if(turn_led_state == ON_DELAY){
    
    if(millis() - current_turn_time > TURN_TRANSITION_DELAY){
      turn_led_state = OFF;
      current_turn_time = millis();
    }

  // ------------------------------------------- TURNING TURN LEDS OFF -------------------------------------------
  } else if(turn_led_state == OFF){

    if(turn_leds_off < TURN_NUM_LEDS) {
      
      if(millis() - current_turn_time > TURN_SHOW_DELAY){

        leds[turn_leds_off].setRGB(0, 0, 0);
        
        FastLED.show();
        turn_leds_off++;
        current_turn_time = millis();
      }
      
    } else{
      turn_led_state = OFF_DELAY;
      turn_leds_off = 0;
      current_turn_time = millis();
      FastLED.show();
    } 

  // ------------------------------------------- TURN LEDS OFF, DELAYING -------------------------------------------
  } else if(turn_led_state == OFF_DELAY){

    if(millis() - current_turn_time > TURN_TRANSITION_DELAY){
      turn_led_state = ON;
      current_turn_time = millis();
    }
  } 
}

void reset_leds(CRGB leds[], long unsigned int &current_turn_time, LED_STATE &turn_led_state, int &turn_leds_on, int &turn_leds_off){
    turn_led_state = ON;
    turn_leds_on = 0;
    turn_leds_off = 0;
    
    fill_solid(leds, TURN_NUM_LEDS, CRGB(0, 0, 0));
    FastLED.show();
    
    current_turn_time = millis();
}
