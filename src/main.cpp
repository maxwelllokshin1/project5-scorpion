#include <Arduino.h>
#include <ESP32Servo.h>
#include "leg.h"

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  if (!startLegs()) { // start the legs up
    Serial.println("startLegs() failed, check config.yaml");
  }
}

void loop() { 
  updateLegs(); // make the leg walk with state machine
}