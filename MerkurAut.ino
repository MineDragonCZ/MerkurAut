#include <Servo.h>
Servo sonarAxisServo;

//R, L
const int MOTORS[] = { 3, 11 };
const int MOTORS_REVERSE[] = { 12, 13 };
const int LINE_SENSORS[] = { A2 };
const int LINE_SENSORS_TRESHOLD[] = { 750 };
// Trig, Echo
const int SONAR_TRIG[] = { 4, 5 };
const int SONAR_ECHO[] = { 6, 7 };
const double SONAR_TRESHOLDS[2][2] = {
  { 5, 10 },
  { 5, 10 }
};

const double RIGHT_CHECK_DISTANCE_TRESHOLD = 2.0;

const int motorSpeed = 140;

const int START_SWITCH_PIN = 10;
const int LEDS[] = { A3, A4, A5 };


bool isOnTrack = false;
long lastLineTest = 0;

void setup() {
  for(int i = 0; i < sizeof(MOTORS); i++){
    pinMode(MOTORS[i], OUTPUT);
  }
  for(int i = 0; i < sizeof(MOTORS_REVERSE); i++){
    pinMode(MOTORS_REVERSE[i], OUTPUT);
  }
  for(int i = 0; i < sizeof(LINE_SENSORS); i++){
    pinMode(LINE_SENSORS[i], INPUT);
  }
  for(int i = 0; i < sizeof(SONAR_TRIG); i++){
    pinMode(SONAR_TRIG[i], OUTPUT);
  }
  for(int i = 0; i < sizeof(SONAR_ECHO); i++){
    pinMode(SONAR_ECHO[i], INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  analogWrite(LEDS[1], 0);
  analogWrite(LEDS[2], 0);
  analogWrite(LEDS[0], 1023);
  if(digitalRead(START_SWITCH_PIN) == LOW){
    stopMotors();
    return;
  }
  analogWrite(LEDS[0], 0);
  analogWrite(LEDS[(isOnTrack ? 2 : 1)], 1023);
  if(millis() - lastLineTest > 5000){
    bool lineSensorActive = testRFSensor(0);
    if(lineSensorActive) 
      isOnTrack = !isOnTrack;
  }
  if(isOnTrack){
    drive();
    delay(50);
  }

  // debugging delay
  delay(2000);
}

// to check if we are going towards the wall or the other way
double lastRightDistance = 0;

void drive(){
  double forwardDistance = getSonarDistance(0);
  double rightDistance = getSonarDistance(1);

  Serial.print("Forward dist: ");
  Serial.println(forwardDistance);
  Serial.print("Right dist: ");
  Serial.println(rightDistance);

  // odchylka od poslední změřené vzdálenosti od steny vpravo je menší nebo rovna přijatelné ochylce -> jede rovně (asi)
  bool goingForward = (abs(rightDistance - lastRightDistance) <= RIGHT_CHECK_DISTANCE_TRESHOLD);

  bool goingRight = (rightDistance < lastRightDistance && !goingForward);
  bool goingLeft = (rightDistance > lastRightDistance && !goingForward);
  lastRightDistance = rightDistance;


  Serial.print("goingForward: ");
  Serial.println((goingForward ? "True" : "False"));
  Serial.print("goingRight: ");
  Serial.println((goingRight ? "True" : "False"));
  Serial.print("goingLeft: ");
  Serial.println((goingLeft ? "True" : "False"));

  bool shouldGoForward = (forwardDistance > SONAR_TRESHOLDS[0][1]);
  bool shouldGoRight = (rightDistance > SONAR_TRESHOLDS[1][1] && (!goingRight));
  bool shouldGoLeft = (rightDistance <= SONAR_TRESHOLDS[1][0] && (!goingLeft || !shouldGoForward));

  if (shouldGoRight) {
    turnRight();
    Serial.println("Right");
    return;
  }
  if(shouldGoLeft){
    turnLeft();
    Serial.println("Left");
    return;
  }
  goForward();
  Serial.println("Forward");
}


bool testRFSensor(int sensorIndex) {
  int value = analogRead(LINE_SENSORS[sensorIndex]);
  return (value > LINE_SENSORS_TRESHOLD[sensorIndex]);
}

double getSonarDistance(int sonarId) {
  digitalWrite(SONAR_TRIG[sonarId], LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG[sonarId], HIGH);
  delayMicroseconds(11);
  digitalWrite(SONAR_TRIG[sonarId], LOW);
  double odezva = pulseIn(SONAR_ECHO[sonarId], HIGH);
  return (odezva / 58.31);
}


void turnRight() {
  analogWrite(MOTORS[0], motorSpeed);
  analogWrite(MOTORS[1], motorSpeed);
  
  //digitalWrite(MOTORS_REVERSE[0], HIGH);
  //digitalWrite(MOTORS_REVERSE[1], LOW);
}

void turnLeft() {
  analogWrite(MOTORS[0], motorSpeed);
  analogWrite(MOTORS[1], motorSpeed);
  
  //digitalWrite(MOTORS_REVERSE[0], LOW);
  //digitalWrite(MOTORS_REVERSE[1], HIGH);
}

void goForward() {
  analogWrite(MOTORS[0], motorSpeed);
  analogWrite(MOTORS[1], motorSpeed);
  
  digitalWrite(MOTORS_REVERSE[0], LOW);
  digitalWrite(MOTORS_REVERSE[1], LOW);
}

void stopMotors(){
  analogWrite(MOTORS[0], 0);
  analogWrite(MOTORS[1], 0);
  
  digitalWrite(MOTORS_REVERSE[0], LOW);
  digitalWrite(MOTORS_REVERSE[1], LOW);
}
