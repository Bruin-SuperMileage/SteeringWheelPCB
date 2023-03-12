#define WIPER_SWITCH 21
#define LIMIT_SWITCH 13
#define IN_1 9
#define IN_2 10
#define IN_3 11
#define IN_4 12

enum WIPER_STATE {EXTEND, RETRACT, EXTEND_DELAY, RETRACT_DELAY};
WIPER_STATE wiper_state = EXTEND;

const int wiper_delay = 2000;
long unsigned int wiper_time = 0;

bool move_up = false;

bool update_device(int input_port, int output_port);
bool read_switch(int port);

void setup(){
  
  Serial.begin(9600);
  
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(WIPER_SWITCH, INPUT);
  pinMode(LIMIT_SWITCH, INPUT);
  
  move_up = false;
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  delay(50);

  wiper_time = millis();
}

void loop(){
  
  if(read_switch(WIPER_SWITCH)){
    if(wiper_state == EXTEND){
      if(!read_switch(LIMIT_SWITCH)|| (move_up && millis()-wiper_time < 2000)){
        set_wipers("EXTEND");
      } 
      else if(move_up && millis()-wiper_time > 2000){
        move_up = 0;
      }
      else{
        set_wipers("STOP");
        wiper_time = millis();
        wiper_state = EXTEND_DELAY;
      }
      
    } else if(wiper_state == RETRACT){
      if(!read_switch(LIMIT_SWITCH) || (move_up && millis()-wiper_time < 2000)) {
        set_wipers("RETRACT");
      } 
      else if(move_up && millis()-wiper_time > 2000){
        move_up = 0;
      }
      else{
        set_wipers("STOP");
        wiper_time = millis();
        wiper_state = RETRACT_DELAY;
      }
      
    } else{
      if(millis() - wiper_time > wiper_delay){
        wiper_state = (wiper_state == EXTEND_DELAY) ? RETRACT : EXTEND;
        wiper_time = millis();
        move_up = 1;
      } else{
        set_wipers("STOP");
      }
    }
  } 
  // ------ NOT WIPING ------
  else{
    if(!read_switch(LIMIT_SWITCH)){
      set_wipers("RETRACT");
    } 
    else
      set_wipers("STOP");
 
    wiper_state = EXTEND; 
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

void set_wipers(String direction){  
  if(direction == "EXTEND"){
    Serial.println("Extending");
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_4, HIGH);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
  } else if(direction == "RETRACT"){
    Serial.println("Retracting");
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  } else if(direction == "STOP"){
    Serial.println("Stopped");
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_4, LOW);
  }
}
