#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    20
#define SHOW_DELAY 100
#define TRANSITION_DELAY 2000

CRGB leds[NUM_LEDS];

long unsigned int current_time;

int leds_on = 0;
int leds_off = 0;

enum LED_STATE {ON, OFF, ON_DELAY, OFF_DELAY};
LED_STATE state = ON;

void setup() {

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  current_time = millis();
}

void loop(){

  // ------------------------------------------- TURNING LEDS ON -------------------------------------------
  if(state == ON) {

    if(leds_on < NUM_LEDS) {
      
      if(millis() - current_time > SHOW_DELAY){
  
        leds[leds_on].setRGB(255, 0, 0);
        FastLED.show();
        leds_on++;
        current_time = millis();
      }
      
    } else{
      state = ON_DELAY;
      leds_on = 0;
      current_time = millis();
    } 

  // ------------------------------------------- LEDS ON, DELAYING -------------------------------------------
  } else if(state == ON_DELAY){
    
    if(millis() - current_time > TRANSITION_DELAY){
      state = OFF;
      current_time = millis();
    }

  // ------------------------------------------- TURNING LEDS OFF -------------------------------------------
  } else if(state == OFF){

    if(leds_off < NUM_LEDS) {
      
      if(millis() - current_time > SHOW_DELAY){
  
        leds[leds_off].setRGB(0, 0, 0);
        FastLED.show();
        leds_off++;
        current_time = millis();
      }
      
    } else{
      state = OFF_DELAY;
      leds_off = 0;
      current_time = millis();
    } 

  // ------------------------------------------- LEDS OFF, DELAYING -------------------------------------------
  } else if(state == OFF_DELAY){

    if(millis() - current_time > TRANSITION_DELAY){
      state = ON;
      current_time = millis();
    }
  }

  // REST OF CODE
}
