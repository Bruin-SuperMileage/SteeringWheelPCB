#define WIPER_SWITCH 21
#define LIMIT_SWITCH 13
#define IN_1 9
#define IN_2 10
#define IN_3 11
#define IN_4 12


enum WIPER_STATE {EXTEND, RETRACT, EXTEND_DELAY, RETRACT_DELAY};

WIPER_STATE wiper_state = RETRACT;

const int wiper_delay = 100;

long unsigned int wiper_time = 0;

bool update_device(int input_port, int output_port);
bool read_switch(int port);

void setup(){

  wiper_time = millis();
  
}

void loop(){


  if(read_switch(WIPER_SWITCH)){

    if(wiper_state == EXTEND){

      if(!read_switch(LIMIT_SWITCH)){
        set_wipers("EXTEND");
      } else{
        wiper_time = millis();
        disable_wipers();
        wiper_state = EXTEND_DELAY;
      }
      
    } else if(wiper_state == RETRACT){

      if(!read_switch(LIMIT_SWITCH)){
        set_wipers("RETRACT");
      } else{
        wiper_time = millis();
        disable_wipers();
        wiper_state = RETRACT_DELAY;
      }
      
    } else{

      if(millis() - wiper_time > wiper_delay){
        wiper_time = millis();
        wiper_state = (wiper_state == EXTEND_DELAY) ? RETRACT : EXTEND;
      } 
    }
  } 

  // ------ NOT WIPING ------
  else{

    if(!read_switch(LIMIT_SWITCH)){
      set_wipers("RETRACT");
    } 
    else
      disable_wipers();
 
    wiper_state = EXTEND;
  }
  
}


bool read_switch(int port){
  return digitalRead(port);
}


bool update_device(int input_port, int output_port){
  bool state = read_switch(input_port);

  digitalWrite(output_port, state);
  return state;
}

// direction: 1 is extend, 0 is retract
void set_wipers(String direction){
  if(direction == "EXTEND"){
    digitalWrite(IN_1, true);
    digitalWrite(IN_4, true);
    digitalWrite(IN_2, false);
    digitalWrite(IN_3, false);
  } else if(direction == "RETRACT"){
    digitalWrite(IN_2, true);
    digitalWrite(IN_3, true);
    digitalWrite(IN_1, false);
    digitalWrite(IN_4, false);
  }
}

void disable_wipers(){
  set_wipers(0, LOW);
  set_wipers(1, LOW);
}
