#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motorFL(1);
AF_DCMotor motorFR(3);
AF_DCMotor motorRL(2);
AF_DCMotor motorRR(4);
Servo _servo;

const int SERVO_PIN = 10;
const int MAX_SPEED = 150;
const int MOTOR_ACTION_DELAY = 1000;
const int MOTOR_REVERSE_DELAY = 500;
const int SERVO_DELAY = 1000;
const int MAX_LENGTH = 70; // The MAX distance the car travels at MAX_SPEED and MOTOR_ACTION_DELAY.

void setup() {  
  init_motors();
  init_servo();
  init_ultrasound();
}

void loop() {  
  int left_distance = get_left_distance();  
  int right_distance = get_right_distance();
  int straight_distance = get_straight_distance();
  move_robot(straight_distance, left_distance, right_distance);   
}

void move_robot(int straight_distance, int left_distance, int right_distance){  // Defines the rules the robot must follow.
  if(straight_distance > MAX_LENGTH){
    go_forward();
  } else if (left_distance > MAX_LENGTH){
    turn_left();  
  } else if (right_distance > MAX_LENGTH){
    turn_right();  
  } else {    
    if(left_distance > right_distance) {
      hard_turn_left();
    } else if (right_distance > left_distance) {
      hard_turn_right();
    } else {
      reverse();
    }
  }
  motor_stop();
}

void init_motors(){  
  motorFL.setSpeed(MAX_SPEED);
  motorFR.setSpeed(MAX_SPEED);
  motorRL.setSpeed(MAX_SPEED);
  motorRR.setSpeed(MAX_SPEED);  

  motorFL.run(RELEASE);
  motorFR.run(RELEASE);
  motorRL.run(RELEASE);
  motorRR.run(RELEASE);
}

void init_servo(){
  _servo.attach(SERVO_PIN);
  _servo.write(90); // look straight at first.
}

void init_ultrasound(){
  pinMode(A4, OUTPUT); //trig pin
  pinMode(A5, INPUT); // echo pin
}

void go_forward(){  
  motorFL.run(FORWARD); 
  motorFR.run(FORWARD);
  motorRL.run(FORWARD); 
  motorRR.run(FORWARD);
  delay(MOTOR_ACTION_DELAY); 
}

void reverse(){
  motorFL.run(BACKWARD); 
  motorFR.run(BACKWARD);
  motorRL.run(BACKWARD); 
  motorRR.run(BACKWARD);
  delay(MOTOR_REVERSE_DELAY); 
}

void turn_right(){  
  motorFL.run(FORWARD); 
  motorFR.run(RELEASE);
  motorRL.run(FORWARD); 
  motorRR.run(RELEASE);
  delay(MOTOR_ACTION_DELAY); 
}

void turn_left(){  
  motorFL.run(RELEASE); 
  motorFR.run(FORWARD);
  motorRL.run(RELEASE); 
  motorRR.run(FORWARD);
  delay(MOTOR_ACTION_DELAY); 
}

void hard_turn_left(){  
  motorFL.run(BACKWARD); 
  motorFR.run(FORWARD);
  motorRL.run(BACKWARD); 
  motorRR.run(FORWARD);
  delay(MOTOR_ACTION_DELAY); 
}

void hard_turn_right(){  
  motorFL.run(FORWARD); 
  motorFR.run(BACKWARD);
  motorRL.run(FORWARD); 
  motorRR.run(BACKWARD);
  delay(MOTOR_ACTION_DELAY); 
}

void motor_stop(){ 
  motorFL.run(RELEASE); 
  motorFR.run(RELEASE);
  motorRL.run(RELEASE); 
  motorRR.run(RELEASE);
}

int get_left_distance(){  
  _servo.write(0);
  delay(SERVO_DELAY);
  return get_distance();
}

int get_straight_distance(){
  _servo.write(90);
  delay(SERVO_DELAY);
  return get_distance();
}

int get_right_distance(){ 
  _servo.write(180);
  delay(SERVO_DELAY);
  return get_distance();
}

int get_distance(){  
  long duration;
  digitalWrite(A4, LOW);
  delayMicroseconds(2);
  digitalWrite(A4, HIGH);
  delayMicroseconds(2);
  duration = pulseIn(A5, HIGH); //speed of sound
  return duration * 0.034 / 2;
}
