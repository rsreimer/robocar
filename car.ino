#include <Servo.h>

#define PIN_RIGHT_MOTOR_ENABLE  5  // ENA
#define PIN_RIGHT_MOTOR_BACK    8  // IN1
#define PIN_RIGHT_MOTOR_FORWARD 9  // IN2

#define PIN_LEFT_MOTOR_ENABLE   6  // ENB
#define PIN_LEFT_MOTOR_BACK     10 // IN3
#define PIN_LEFT_MOTOR_FORWARD  11 // IN4

#define PIN_SONAR_ECHO          A0
#define PIN_SONAR_TRIGGER       A1

#define PIN_SERVO               3

#define SERVO_FORWARD           110
#define SERVO_LEFT              SERVO_FORWARD + 60
#define SERVO_RIGHT             SERVO_FORWARD - 60
#define SERVO_DELAY             250

#define MOTOR_PWM               125
#define MOTOR_FORWARD           1
#define MOTOR_STOP              0
#define MOTOR_BACKWARD          -1

Servo servo;

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_LEFT_MOTOR_ENABLE, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_BACK, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_FORWARD, OUTPUT);

  pinMode(PIN_RIGHT_MOTOR_ENABLE, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_BACK, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_FORWARD, OUTPUT);
  
  pinMode(PIN_SONAR_ECHO, INPUT);
  pinMode(PIN_SONAR_TRIGGER, OUTPUT);
  
  servo.attach(PIN_SERVO);

  analogWrite(PIN_LEFT_MOTOR_ENABLE, MOTOR_PWM);
  analogWrite(PIN_RIGHT_MOTOR_ENABLE, MOTOR_PWM);
}

void drive(int left, int right, int steps) {
  digitalWrite(PIN_LEFT_MOTOR_BACK, left == MOTOR_FORWARD ? LOW : HIGH);
  digitalWrite(PIN_LEFT_MOTOR_FORWARD, left == MOTOR_BACKWARD ? LOW : HIGH);

  digitalWrite(PIN_RIGHT_MOTOR_BACK, right == MOTOR_FORWARD ? LOW : HIGH);
  digitalWrite(PIN_RIGHT_MOTOR_FORWARD, right == MOTOR_BACKWARD ? LOW : HIGH);
  
  delay(steps);
}

void driveForward(int steps)  { drive(MOTOR_FORWARD,  MOTOR_FORWARD,  steps); }
void driveBack(int steps)     { drive(MOTOR_BACKWARD, MOTOR_BACKWARD, steps); }
void turnRight(int steps)     { drive(MOTOR_FORWARD,  MOTOR_BACKWARD, steps); }
void turnLeft(int steps)      { drive(MOTOR_BACKWARD, MOTOR_FORWARD,  steps); }
void driveStop()              { drive(MOTOR_STOP,     MOTOR_STOP,     100  ); }

float measure(int direction) {
  servo.write(direction);
  delay(SERVO_DELAY);
  
  digitalWrite(PIN_SONAR_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_SONAR_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_SONAR_TRIGGER, LOW);

  return pulseIn(PIN_SONAR_ECHO, HIGH) / 5.8 / 10;
}

void loop() {
  float distance_forward = measure(SERVO_FORWARD);
  
  if (distance_forward < 20) {
    driveStop();
    driveBack(600);
  }
  else if (distance_forward < 35) {
    driveStop();
    
    float distance_left = measure(SERVO_LEFT);
    float distance_right = measure(SERVO_RIGHT);

    if (distance_left < 10 && distance_right < 10) {
      driveBack(900);
      turnRight(100);
    }
    else if (distance_left > distance_right) {
      driveBack(300);
      turnLeft(100);
    }
    else if (distance_left <= distance_right) {
      driveBack(300);
      turnRight(100);
    }
  } else {
    driveForward(15); 
  }
}