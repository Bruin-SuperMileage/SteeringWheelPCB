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


void setup() {
  // put your setup code here, to run once:

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

}

void loop() {

  // MANAGE STATUS LEDS
  controlHeadlightOut();
  controlLeftTurnOut();
  controlRightTurnOut();
  controlBrakeOut();
  controlHorn();
}


// SWITCH MANAGING FUNCTIONS

bool readHeadlightSwitch(){
  return digitalRead(HEADLIGHT_SWITCH);
}

bool readLeftTurnSwitch(){
  return digitalRead(LEFT_TURN_SWITCH);
}

bool readRightTurnSwitch(){
  return digitalRead(RIGHT_TURN_SWITCH);
}

bool readBrakeSwitch(){
  return digitalRead(BRAKE_SWITCH);
}

bool readHornSwitch(){
  return digitalRead(HORN_SWITCH);
}


void controlHeadlightOut(){
  bool state = readHeadlightSwitch(); // set this eq to a var

  Serial.print("Headlight state: ");
  Serial.println(state);
  
  digitalWrite(HEADLIGHT_MOSFET, state);
}

void controlLeftTurnOut(){
  bool state = readLeftTurnSwitch(); // set this eq to a var

  Serial.println("Left turn blinker state: ");
  Serial.println(state);
  
  digitalWrite(LEFT_TURN_MOSFET, state);
}

void controlRightTurnOut(){
  bool state = readRightTurnSwitch(); // set this eq to a var

  Serial.println("Right turn blinker state: ");
  Serial.println(state);
  
  digitalWrite(RIGHT_TURN_MOSFET, state);
}

void controlBrakeOut(){
  bool state = readBrakeSwitch(); // set this eq to a var

  Serial.println("Brake state: ");
  Serial.println(state);
  
  digitalWrite(BRAKE_MOSFET, state);
}

void controlHorn(){
  bool state = readHornSwitch();
  
  Serial.println("Horn state: ");
  Serial.println(state);
  
  digitalWrite(HORN_MOSFET, state);
}
