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


// Container class for our variables
struct LEDStateStrip {
  CRGB leds[TURN_NUM_LEDS];
  int leds_on;
  int leds_off;
  long unsigned int current_time;
  LED_STATE state;
};

LEDStateStrip left_turn_leds;
LEDStateStrip right_turn_leds;


// --------------- PRE-SET-UP ---------------

bool read_switch(int port);
bool update_device(int port);

void reset_leds(LEDStateStrip &strip);
void set_leds_turn(LEDStateStrip &strip);

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


  // LED STRIP SETUP

  left_turn_leds.leds_on = 0;
  left_turn_leds.leds_off = 0;
  right_turn_leds.leds_on = 0;
  right_turn_leds.leds_off = 0;
  
  left_turn_leds.state = ON;
  right_turn_leds.state = ON;
  

  // BRAKE LED SET-UP //

  //FastLED.addLeds<WS2812, BRAKE_LED_PIN, GRB>(brake_leds, BRAKE_NUM_LEDS);

  // TURN LED SET-UP //

  FastLED.addLeds<WS2812, FRONT_LEFT_TURN_LED_PIN, GRB>(left_turn_leds.leds, TURN_NUM_LEDS);
  FastLED.addLeds<WS2812, FRONT_RIGHT_TURN_LED_PIN, GRB>(right_turn_leds.leds, TURN_NUM_LEDS);
  
  right_turn_leds.current_time = millis();
  right_turn_leds.current_time = millis();
}

// ------------------------------------------------------------------------ MAIN LOOP ---------------------------------------------------------------------------
void loop() {

  // State printing
  Serial.print("Hazard Switch: ");
  Serial.print(read_switch(HAZARD_SWITCH));
  Serial.print(", Left Switch: ");
  Serial.print(read_switch(LEFT_TURN_SWITCH));
  Serial.print(", Right Switch: ");
  Serial.println(read_switch(RIGHT_TURN_SWITCH));

  Serial.print("Headlight Switch: ");
  Serial.println(read_switch(HEADLIGHT_SWITCH));

  Serial.print("Horn Switch: ");
  Serial.println(read_switch(HORN_SWITCH));

  delay(50);
  
  // SETTING TURN + HAZARD LIGHTS
  if(read_switch(HAZARD_SWITCH)){
    set_leds_turn(left_turn_leds);
    set_leds_turn(right_turn_leds);
  } 
  
  else if(read_switch(LEFT_TURN_SWITCH)){
    reset_leds(right_turn_leds);
    set_leds_turn(left_turn_leds);
  } 
  
  else if(read_switch(RIGHT_TURN_SWITCH)){
    reset_leds(left_turn_leds);
    set_leds_turn(right_turn_leds);
  } 
  
  else {
    reset_leds(left_turn_leds);
    reset_leds(right_turn_leds);
  }

  // SETTING HEADLIGHTS
  update_device(HEADLIGHT_SWITCH, HEADLIGHT_MOSFET);

  // SETTING HORN
  update_device(HORN_SWITCH, HORN_MOSFET);
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

void set_leds_turn(LEDStateStrip &strip){

  // ------------------------------------------- TURNING TURN LEDS ON -------------------------------------------
  if(strip.state == ON) {

    if(strip.leds_on < TURN_NUM_LEDS) {
      
      if(millis() - strip.current_time > TURN_SHOW_DELAY){

        strip.leds[strip.leds_on].setRGB((int)(255 * brightness), 0, 0);
        FastLED.show();
        strip.leds_on++;
        strip.current_time = millis();
      }
      
    } else{
      strip.state = ON_DELAY;
      strip.leds_on = 0;
      strip.current_time = millis();
      FastLED.show();
    } 

  // ------------------------------------------- TURN LEDS ON, DELAYING -------------------------------------------
  } else if(strip.state == ON_DELAY){
    
    if(millis() - strip.current_time > TURN_TRANSITION_DELAY){
      strip.state = OFF;
      strip.current_time = millis();
    }

  // ------------------------------------------- TURNING TURN LEDS OFF -------------------------------------------
  } else if(strip.state == OFF){

    if(strip.leds_off < TURN_NUM_LEDS) {
      
      if(millis() - strip.current_time > TURN_SHOW_DELAY){

        strip.leds[strip.leds_off].setRGB(0, 0, 0);
        
        FastLED.show();
        strip.leds_off++;
        strip.current_time = millis();
      }
      
    } else{
      strip.state = OFF_DELAY;
      strip.leds_off = 0;
      strip.current_time = millis();
      FastLED.show();
    } 

  // ------------------------------------------- TURN LEDS OFF, DELAYING -------------------------------------------
  } else if(strip.state == OFF_DELAY){

    if(millis() - strip.current_time > TURN_TRANSITION_DELAY){
      strip.state = ON;
      strip.current_time = millis();
    }
  } 
}


void reset_leds(LEDStateStrip &strip){
    strip.state = ON;
    strip.leds_on = 0;
    strip.leds_off = 0;
    
    fill_solid(strip.leds, TURN_NUM_LEDS, CRGB(0, 0, 0));
    FastLED.show();
    
    strip.current_time = millis();
}
