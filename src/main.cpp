#include <Arduino.h>
#include <ESP32Servo.h>

// //////// REMINDER: ALL SERVOS ARE 180 SERVOS \\\\\\\\\

Servo myservo;
const int servo_pin = 32; // gpio pin

enum State {
  STATE_CENTER,
  STATE_UP,
  STATE_DOWN
};

State current_state = STATE_CENTER;
unsigned long state_starts = 0;

// just move to that state
void changeState(State next){
  current_state = next;

  state_starts = millis();

  switch (next){
    case STATE_CENTER: myservo.write(90); break;
    case STATE_UP: myservo.write(180); break;
    case STATE_DOWN: myservo.write(0); break;
  }
}

void setup() {
  // put your setup code here, to run once:
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);


  myservo.setPeriodHertz(50); // 50hz servo
  myservo.attach(servo_pin, 500, 2500); // attach servo with min max pulse

  changeState(STATE_CENTER);
}

void loop() {
  unsigned long elapsed = millis() - state_starts;

  switch (current_state){
    case STATE_CENTER:
      if (elapsed >= 1000) changeState(STATE_UP); break;
    case STATE_UP:
      if (elapsed >= 1000) changeState(STATE_DOWN); break;
    case STATE_DOWN:
      if (elapsed >= 1000) changeState(STATE_CENTER); break;
  }
  
}