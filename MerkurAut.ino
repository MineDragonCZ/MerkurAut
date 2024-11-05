#include <Servo.h>
Servo myservo;


const int MOTORS[] = {10, 9, 6, 5};
const int LINE_SENSORS[] = {A0, A2};
const int LINE_SENSORS_TRESHOLD[] = {500, 500};

void setup(){
  myservo.attach(11);
  for(int i = 0; i < sizeof(MOTORS); i++){
    pinMode(MOTORS[i], OUTPUT);
  }
  for(int i = 0; i < sizeof(LINE_SENSORS); i++){
    pinMode(LINE_SENSORS[i], INPUT);
  }

  Serial.begin(9600);
}

void loop(){
  // TODO: Write good algorithm to drive the fking car
}