#include <Servo.h>
Servo myservo;

                    //LF, LB, RF, RB
const int MOTORS[] = {10, 9, 6, 5};
const int LINE_SENSORS[] = {A5, A4};
const int LINE_SENSORS_TRESHOLD[] = {750, 750};

const int motorSpeed = 140;

void setup(){
  //myservo.attach(11);
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
    Serial.println("Forward");
    return;
  }
  if(rightSensor){
    turnRight();
    Serial.println("Right");
    return;
  }
  turnLeft();
  Serial.println("Left");
}


bool testRFSensor(int sensorIndex) {
  int value = analogRead(LINE_SENSORS[sensorIndex]);
  Serial.println(value);
  return (value > LINE_SENSORS_TRESHOLD[sensorIndex]);
}

void turnRight() {
  for(int i = 0; i < sizeof(MOTORS); i++){
    analogWrite(MOTORS[i], (!(i == 1 || i == 2) ? motorSpeed : 0));
  }
}

void turnLeft() {
  for(int i = 0; i < sizeof(MOTORS); i++){
    analogWrite(MOTORS[i], ((i == 1 || i == 2) ? motorSpeed : 0));
  }
}

void goForward(){
  for(int i = 0; i < sizeof(MOTORS); i++){
    analogWrite(MOTORS[i], (i % 2 == 0 ? motorSpeed : 0));
  }
}



