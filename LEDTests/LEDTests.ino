#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    20

CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
}

void loop() {
  
  for(int i = 0; i < NUM_LEDS; i++){

    leds[i].setRGB(255, 0, 0);
    FastLED.show();

    if(i != NUM_LEDS - 1){
      delay(100);
    }
  }

  delay(2000);

  for(int i = 0; i < NUM_LEDS; i++)
    leds[i].setRGB(0, 0, 0);

  FastLED.show();
  
  delay(2000);
}
