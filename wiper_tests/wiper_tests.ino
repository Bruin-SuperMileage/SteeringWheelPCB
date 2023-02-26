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
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(WIPER_SWITCH, INPUT);

  digitalWrite(IN_2, LOW);
  delay(50);
}

void loop(){

  /*Serial.println(read_switch(WIPER_SWITCH));
  
  if(read_switch(WIPER_SWITCH)){

    if(wiper_state == EXTEND){

      if(!read_switch(LIMIT_SWITCH)){
        set_wipers("EXTEND");
      } else{
        disable_wipers();
        wiper_time = millis();
        wiper_state = EXTEND_DELAY;
      }
      
    } else if(wiper_state == RETRACT){

      if(!read_switch(LIMIT_SWITCH)){
        set_wipers("RETRACT");
      } else{
        disable_wipers();
        wiper_time = millis();
        wiper_state = RETRACT_DELAY;
      }
      
    } else{

      if(millis() - wiper_time > wiper_delay){
        wiper_time = millis();
        wiper_state = (wiper_state == EXTEND_DELAY) ? RETRACT : EXTEND;
      } else{
        disable_wipers();
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
  }*/

  
  /*Serial.println(read_switch(WIPER_SWITCH));
  if(read_switch(WIPER_SWITCH)){
    disable_wipers();
    delay(50);
    set_wipers("EXTEND");
    delay(5000);
    disable_wipers();
    delay(50);
    set_wipers("RETRACT");
    delay(5000);
  }
  else {
    disable_wipers();
  }*/

  if(read_switch(WIPER_SWITCH)){
    digitalWrite(IN_1, HIGH);
  } else {
    digitalWrite(IN_1, LOW);
  }

  delay(50);
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
    Serial.println("Extending");
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_4, HIGH);
  } else if(direction == "RETRACT"){
    Serial.println("Retracting");
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
  } else if(direction == "STOP"){
    Serial.println("Stopped");
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  }
}

void disable_wipers(){
  set_wipers("STOP");
}
