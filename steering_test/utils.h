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
          strip.leds[strip.leds_on].setRGB((int)(255 * brightness), (int)(100 * brightness), 0);
        else
          strip.leds[TURN_NUM_LEDS - 1 - strip.leds_on].setRGB((int)(255 * brightness), (int)(100 * brightness), 0);
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
  if(!wiper.home)
  {
    reset_wiper(wiper);
    return;
  }
  if(wiper.state == EXTEND){
    
    if(wiper.return_time == -1)
      wiper.return_time = millis();
    if(millis() - wiper.return_time < wiper.mosfet_delay)
    {
        set_wiper_mosfets("STOP");
        Serial.println("end delay");
        wiper.current_time = wiper.return_time + wiper.mosfet_delay;
    }
    else
    {    
      if(millis()-wiper.current_time<2500)
      {
        set_wiper_mosfets("EXTEND");
      }
      else
      {
        set_wiper_mosfets("STOP");
        wiper.state = EXTEND_DELAY;
        wiper.current_time = millis();
      }
    }
    
  } else if(wiper.state == RETRACT){
    if(millis()-wiper.current_time<2500)
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
  wiper.flag = true;
  
}

void reset_wiper(WiperController &wiper){
  
  if(wiper.flag)
  {
    wiper.home = false;
    wiper.flag = false;
    wiper.return_time = -1;
  }
  if(wiper.home)
    return;
  if(digitalRead(LIMIT_SWITCH))
  {        
    set_wiper_mosfets("STOP");
    wiper.home = true;
    wiper.reset_state = START_DELAY;
    wiper.return_time = -1;
  }
  else
  {
    if(wiper.reset_state == START_DELAY)
    {
      Serial.println("start delay");
      if(wiper.return_time == -1)
        wiper.return_time = millis();
    
      if(millis() - wiper.return_time < wiper.mosfet_delay)
        set_wiper_mosfets("STOP");
      else
      {
        wiper.reset_state = HOMING;
        wiper.return_time = -1;
      }
    }
    else if(wiper.reset_state == HOMING)
    {
      Serial.println("homing");
      set_wiper_mosfets("RETRACT");
      wiper.return_time = -1;
    }   
    wiper.home = false;
  }
  wiper.state = EXTEND;
  
}

void set_wiper_mosfets(String direction){
  if(direction == "EXTEND"){
    Serial.println("extending");
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  } else if(direction == "RETRACT"){
    Serial.println("retracting");
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_4, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
  } else if(direction == "STOP"){
    Serial.println("stopped");
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  }
}

#endif
