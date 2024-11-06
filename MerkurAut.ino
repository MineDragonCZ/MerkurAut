#include <Servo.h>
Servo myservo;

                    //LF, LB, RF, RB
const int MOTORS[] = {10, 9, 6, 5};
const int LINE_SENSORS[] = {A5, A4};
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
  bool middleSensor = testRFSensor(0);
  bool rightSensor = testRFSensor(1);
  if(middleSensor){
    goForward();
    return;
  }
  if(rightSensor){
    turnRight();
    return;
  }
  turnLeft();
}


bool testRFSensor(int sensorIndex) {
  int value = analogRead(LINE_SENSORS[sensorIndex]);
  return (value <= LINE_SENSORS_TRESHOLD[sensorIndex]);
}

void turnRight() {
  for(int i = 0; i < sizeof(MOTORS); i++){
    digitalWrite(MOTORS[i], (!(i == 1 || i == 2) ? HIGH : LOW));
  }
}

void turnLeft() {
  for(int i = 0; i < sizeof(MOTORS); i++){
    digitalWrite(MOTORS[i], ((i == 1 || i == 2) ? HIGH : LOW));
  }
}

void goForward(){
  for(int i = 0; i < sizeof(MOTORS); i++){
    digitalWrite(MOTORS[i], (i % 2 == 0 ? HIGH : LOW));
  }
}



