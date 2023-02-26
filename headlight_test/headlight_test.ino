#define HEADLIGHT_SWITCH 15
#define HEADLIGHT_MOSFET 0
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(HEADLIGHT_SWITCH, INPUT);
  pinMode(HEADLIGHT_MOSFET, OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(HEADLIGHT_SWITCH));
  if(digitalRead(HEADLIGHT_SWITCH))
  {
    digitalWrite(HEADLIGHT_MOSFET,HIGH);
  }
  else
  {
    digitalWrite(HEADLIGHT_MOSFET,LOW);
  }
  delay(50);
}
