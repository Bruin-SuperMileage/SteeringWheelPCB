#ifndef UTILS_H
#define UTILS_H
#include "constants.h"

// --------------- PRE-SET-UP ---------------
bool read_switch(int port);
bool update_device(int port);
void reset_leds(LEDStateStrip &strip);
void set_leds_turn(LEDStateStrip &strip);
void set_wiper_mosfets(String direction);
void reset_wiper(WiperController &wiper);
void set_wiper(WiperController &wiper);

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
void set_leds_turn(LEDStateStrip &strip, String dir){
  // ------------------------------------------- TURNING TURN LEDS ON -------------------------------------------
  if(strip.state == ON) {
    if(strip.leds_on < TURN_NUM_LEDS) {
      if(millis() - strip.current_time > TURN_SHOW_DELAY){
        if(dir == "FRONT")
          strip.leds[strip.leds_on].setRGB((int)(255 * brightness), 0, 0);
        else
          strip.leds[TURN_NUM_LEDS - 1 - strip.leds_on].setRGB((int)(255 * brightness), 0, 0);
        strip.leds_on++;
        FastLED.show();
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
        if(dir == "FRONT")
          strip.leds[strip.leds_off].setRGB(0, 0, 0);
        else
          strip.leds[TURN_NUM_LEDS - 1 - strip.leds_off].setRGB(0, 0, 0);
        strip.leds_off++;
        FastLED.show();
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

// WIPER STATE MACHINE
void set_wiper(WiperController &wiper){

  // Reset vars for homing
  wiper.homed = false;

  // State machine
  if(wiper.state == EXTEND){
    if(millis() - wiper.current_time < wiper.wipe_time)
      set_wiper_mosfets("EXTEND");
    else{
      set_wiper_mosfets("STOP");
      wiper.current_time = millis();
      wiper.state = EXTEND_DELAY;
    }
  } else if(wiper.state == RETRACT){
    if(millis() - wiper.current_time < wiper.wipe_time)
      set_wiper_mosfets("RETRACT");
    else {
      set_wiper_mosfets("STOP");
      wiper.current_time = millis();
      wiper.state = RETRACT_DELAY;
    }
  } else{
    if(millis() - wiper.current_time > wiper.mosfet_delay){
      wiper.state = (wiper.state == EXTEND_DELAY) ? RETRACT : EXTEND;
      wiper.current_time = millis();
    }
    else
      set_wiper_mosfets("STOP");
  }
}

void reset_wiper(WiperController &wiper){

  if(wiper.homed)
    return;

  // Already at home position, send immediately to final delay
  if(wiper.state == RETRACT_DELAY){
    wiper.reset_state = END_DELAY;
    wiper.current_time = millis();
  }


  // Initial delay to prevent shoot-through
  if(wiper.reset_state == START_DELAY){

    // Using return_time to timekeep, ugly code, ignore
    if(wiper.return_time == -1)
      wiper.return_time = millis();
    
    if(millis() - wiper.return_time < wiper.mosfet_delay)
      set_wiper_mosfets("STOP");
    else {
      set_wiper_mosfets("STOP");
      wiper.reset_state = HOMING;
      wiper.return_time = -1;

      // If wiper is completely extended, change current_time and state so that it'll return to home in HOMING state
      if(wiper.state == EXTEND_DELAY)
        wiper.current_time = millis() - wiper.mosfet_delay - wiper.wipe_time;
        wiper.state = EXTEND;
    }
  }


  // Return to home position
  if(wiper.reset_state == HOMING){

    // First time this code is run, saves the amount of time wiper has been extended
    if(wiper.return_time == -1) {
      wiper.return_time = millis() - wiper.mosfet_delay - wiper.current_time;
      wiper.current_time = millis();
    }
    
    // Time has been saved, return to home position

    // Calculates time to return
    // Either return_time:          wiper has extended a certain amount, needs to return the same amount
    // Or wipe_time - return_time:  wiper has returned a certain amount, needs to return whatever distance is left
    long unsigned int time_to_move = (wiper.state == EXTEND) ? wiper.return_time : wiper.wipe_time - wiper.return_time;

    // Returns until it meets time_to_move (has arrived at home position)
    if(millis() - wiper.current_time < time_to_move)
      set_wiper_mosfets("RETRACT");

    // At home, state switched so that the next if statement resets all other variables
    else{
      wiper.return_time = -1;
      wiper.reset_state = END_DELAY;
      wiper.current_time = millis();
    }
  }
  

  // Delay at the end of movement to prevent shoot-through
  if(wiper.reset_state == END_DELAY){
    if(millis() - wiper.current_time < wiper.mosfet_delay)
      set_wiper_mosfets("STOP");
    else
      wiper.reset_state = RESET;
  }

  // Resets all variables, wiper is at home position
  if(wiper.reset_state == RESET){
    set_wiper_mosfets("STOP");
    wiper.homed = true;
    wiper.state = EXTEND;
    wiper.return_time = -1;
    wiper.reset_state = START_DELAY;
    wiper.current_time = millis();
  }
}

void set_wiper_mosfets(String direction){
  if(direction == "EXTEND"){
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_4, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
  } else if(direction == "RETRACT"){
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  } else if(direction == "STOP"){
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  }
}

#endif
