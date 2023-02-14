#include <FastLED.h>

// INPUTS //

#define DAQ_BUTTON 14
#define CAN_BUTTON -1
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

#define BRAKE_LED_PIN     1
#define BRAKE_NUM_LEDS    10

CRGB brake_leds[BRAKE_NUM_LEDS];

// --------------- TURN LED CONTROL ---------------

#define LEFT_TURN_LED_PIN          1//50
#define RIGHT_TURN_LED_PIN         2
#define TURN_NUM_LEDS              10
#define TURN_SHOW_DELAY            100
#define TURN_TRANSITION_DELAY      500

CRGB left_leds[TURN_NUM_LEDS];
CRGB right_leds[TURN_NUM_LEDS];

long unsigned int current_turn_time;

int turn_leds_on = 0;
int turn_leds_off = 0;

LED_STATE turn_led_state = ON;

int turn_state = 0;


// --------------- SET-UP ---------------

bool read_switch(int port);
bool update_device(int port);


CLEDController *controllers[1];

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

  FastLED.addLeds<WS2812, BRAKE_LED_PIN, GRB>(brake_leds, BRAKE_NUM_LEDS);

  // TURN LED SET-UP //

  //FastLED.addLeds<WS2812, LEFT_TURN_LED_PIN, GRB>(left_leds, TURN_NUM_LEDS);
  //FastLED.addLeds<WS2812, RIGHT_TURN_LED_PIN, GRB>(right_leds, TURN_NUM_LEDS);
  
  current_turn_time = millis();
}


// --------------- MAIN LOOP ---------------
void loop() {


  // Turn and turn LED setting

  /*
  if(read_switch(LEFT_TURN_SWITCH) && read_switch(RIGHT_TURN_SWITCH))
    turn_state = 3 - turn_state;
  else if(read_switch(LEFT_TURN_SWITCH))
    turn_state = 1;
  else if(read_switch(RIGHT_TURN_SWITCH))
    turn_state = 2;
  else turn_state = 0;*/  // This code is for when we integrate both turning lights


  /*
  if(read_switch(LEFT_TURN_SWITCH)) 
    turn_state = 1;
  else turn_state = 0;
  

  //Serial.println(turn_state);

  if(turn_state != 0){
  
    // ------------------------------------------- TURNING TURN LEDS ON -------------------------------------------
    if(turn_led_state == ON) {
  
      if(turn_leds_on < TURN_NUM_LEDS) {
        
        if(millis() - current_turn_time > TURN_SHOW_DELAY){

          if(turn_state == 1){
            left_leds[turn_leds_on].setRGB((int)(255 * brightness), 0, 0);
            //right_leds[turn_leds_on].setRGB(0, 0, 0);
          } else {
            right_leds[turn_leds_on].setRGB((int)(255 * brightness), 0, 0);
            left_leds[turn_leds_on].setRGB(0, 0, 0);
          }
          
          FastLED.show();
          turn_leds_on++;
          current_turn_time = millis();
        }
        
      } else{
        turn_led_state = ON_DELAY;
        turn_leds_on = 0;
        current_turn_time = millis();
        //FastLED.show();
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

          if(turn_state == 1)
            left_leds[turn_leds_off].setRGB(0, 0, 0);
          else
            right_leds[turn_leds_off].setRGB(0, 0, 0);
           
          FastLED.show();
          turn_leds_off++;
          current_turn_time = millis();
        }
        
      } else{
        turn_led_state = OFF_DELAY;
        turn_leds_off = 0;
        current_turn_time = millis();
        //FastLED.show();
      } 
  
    // ------------------------------------------- TURN LEDS OFF, DELAYING -------------------------------------------
    } else if(turn_led_state == OFF_DELAY){
  
      if(millis() - current_turn_time > TURN_TRANSITION_DELAY){
        turn_led_state = ON;
        current_turn_time = millis();
      }
    }
  } else{

    turn_led_state = ON;
    turn_leds_on = 0;
    turn_leds_off = 0;
    
    for(int i = 0; i < TURN_NUM_LEDS; i++) {
      left_leds[i].setRGB(0, 0, 0);
      //right_leds[turn_leds_off].setRGB(0, 0, 0);
    }
    
    FastLED.show();
    current_turn_time = millis();
  }*/

  // End of turn LED stuff

  //Serial.println(read_switch(LEFT_TURN_SWITCH));

  // Set brake LEDs
  
  if(read_switch(LEFT_TURN_SWITCH)){
    Serial.println("Should be turning leds on");
    uint8_t gBrightness = 128;
    fill_solid(brake_leds, BRAKE_NUM_LEDS, CRGB((int)(255 * brightness),0,0));
    controllers[0] -> showLeds(gBrightness);
    Serial.println("Turned on");
    //FastLED.show();
  } else {
    uint8_t gBrightness = 128;
    fill_solid(brake_leds, BRAKE_NUM_LEDS, CRGB(0,0,0));
    controllers[0] -> showLeds(gBrightness);
    //FastLED.show();
    Serial.println("Turning leds off");
  }
  
  // End of brake LED stuff
}


// SWITCH & OUTPUT MANAGING FUNCTIONS

bool read_switch(int port){
  return digitalRead(port);
}


bool update_device(int port){
  bool state = read_switch(port);

  digitalWrite(port, state);
  return state;
}
