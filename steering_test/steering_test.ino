#include <FastLED.h>
#include "utils.h"
#include "constants.h"

LEDStateStrip left_turn_leds;
LEDStateStrip right_turn_leds;
LEDStateStrip back_left_turn_leds;
LEDStateStrip back_right_turn_leds;

WiperController wiper_controller;

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
  pinMode(LIMIT_SWITCH, INPUT);
  
  // OUTPUTS //
  pinMode(HEADLIGHT_MOSFET, OUTPUT);
  pinMode(BRAKE_MOSFET, OUTPUT);
  pinMode(HORN_MOSFET, OUTPUT);
  pinMode(MASTER_TURN_MOSFET, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  
  // LED STRIP SETUP
  left_turn_leds.leds_on = 0;
  left_turn_leds.leds_off = 0;
  right_turn_leds.leds_on = 0;
  right_turn_leds.leds_off = 0;
  back_left_turn_leds.leds_on = 0;
  back_left_turn_leds.leds_off = 0;
  back_right_turn_leds.leds_on = 0;
  back_right_turn_leds.leds_off = 0;
  left_turn_leds.state = ON;
  right_turn_leds.state = ON;
  back_left_turn_leds.state = ON;
  back_right_turn_leds.state = ON;
  
  // BRAKE LED SET-UP //
  FastLED.addLeds<WS2812, BRAKE_LED_PIN, GRB>(brake_leds, BRAKE_NUM_LEDS);
  
  // TURN LED SET-UP //
  FastLED.addLeds<WS2812, FRONT_LEFT_TURN_LED_PIN, GRB>(left_turn_leds.leds, TURN_NUM_LEDS);
  FastLED.addLeds<WS2812, FRONT_RIGHT_TURN_LED_PIN, GRB>(right_turn_leds.leds, TURN_NUM_LEDS);
  FastLED.addLeds<WS2812, BACK_LEFT_TURN_LED_PIN, GRB>(back_left_turn_leds.leds, TURN_NUM_LEDS);
  FastLED.addLeds<WS2812, BACK_RIGHT_TURN_LED_PIN, GRB>(back_right_turn_leds.leds, TURN_NUM_LEDS);
  
  // WIPER SET-UP //
  wiper_controller.state = EXTEND;
  wiper_controller.move_up = true;
  
  // RESET WIPER MOSFETS
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);

  // SET MASTER MOSFET ON (TO TURN LEDS OFF AT START)
  digitalWrite(MASTER_TURN_MOSFET, HIGH);
  
  delay(50);
  
  // SET TIMER VARS //
  left_turn_leds.current_time = millis();
  right_turn_leds.current_time = millis();
  back_left_turn_leds.current_time = millis();
  back_right_turn_leds.current_time = millis();
  wiper_controller.current_time = millis();
}
// ------------------------------------------------------------------------ MAIN LOOP ---------------------------------------------------------------------------
void loop() {
  // State printing
//  Serial.print("Hazard Switch: ");
//  Serial.print(read_switch(HAZARD_SWITCH));
//  Serial.print(", Left Switch: ");
//  Serial.print(read_switch(LEFT_TURN_SWITCH));
//  Serial.print(", Right Switch: ");
//  Serial.println(read_switch(RIGHT_TURN_SWITCH));
//
//  Serial.print("Headlight Switch: ");
//  Serial.println(read_switch(HEADLIGHT_SWITCH));
//
//
//  Serial.print("Wiper Switch: ");
//  Serial.println(read_switch(WIPER_SWITCH));
//
//  Serial.print("Horn Switch: ");
//  Serial.println(read_switch(HORN_SWITCH));
//
//  Serial.print("DAQ Switch: ");
//  Serial.println(read_switch(DAQ_BUTTON));

  // SETTING TURN + HAZARD LIGHTS (OR BOTH TURNS LIGHTS ON AT ONCE)
  if(read_switch(HAZARD_SWITCH) || (read_switch(LEFT_TURN_SWITCH) && read_switch(RIGHT_TURN_SWITCH))){
    digitalWrite(MASTER_TURN_MOSFET, HIGH);
    set_leds_turn(left_turn_leds, "FRONT");
    set_leds_turn(right_turn_leds, "FRONT");
    set_leds_turn(back_left_turn_leds, "BACK");
    set_leds_turn(back_right_turn_leds, "BACK");
  }

  else if(read_switch(LEFT_TURN_SWITCH)){
    digitalWrite(MASTER_TURN_MOSFET, HIGH);
    reset_leds(right_turn_leds);
    set_leds_turn(left_turn_leds, "FRONT");
    reset_leds(back_right_turn_leds);
    set_leds_turn(back_left_turn_leds, "BACK");
  }
  
  else if(read_switch(RIGHT_TURN_SWITCH)){
    digitalWrite(MASTER_TURN_MOSFET, HIGH);
    reset_leds(left_turn_leds);
    set_leds_turn(right_turn_leds, "FRONT");
    reset_leds(back_left_turn_leds);
    set_leds_turn(back_right_turn_leds, "BACK");
  }
  
 else {
    reset_leds(left_turn_leds);
    reset_leds(right_turn_leds);
    reset_leds(back_left_turn_leds);
    reset_leds(back_right_turn_leds);
    digitalWrite(MASTER_TURN_MOSFET, LOW);
  }

  
  // SETTING BRAKE LIGHTS
  digitalWrite(MASTER_TURN_MOSFET, HIGH);
  if(read_switch(BRAKE_SWITCH))
    fill_solid(brake_leds, BRAKE_NUM_LEDS, CRGB(255, 0, 0));
  else
    fill_solid(brake_leds, BRAKE_NUM_LEDS, CRGB(0, 0, 0));
  
  FastLED.show();
  
  // SETTING WIPER
  if(read_switch(WIPER_SWITCH))
    set_wiper(wiper_controller);
  else
    reset_wiper(wiper_controller);
  
  // SETTING HEADLIGHTS
  update_device(HEADLIGHT_SWITCH, HEADLIGHT_MOSFET);
  
  // SETTING HORN
  update_device(HORN_SWITCH, HORN_MOSFET);
  delay(50);
}
