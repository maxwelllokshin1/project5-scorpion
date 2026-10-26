#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

// //////// REMINDER: ALL SERVOS ARE 180 SERVOS \\\\\\\\\

// shoulder = horizontal swing (forward/backward)
// elbow + wrist = vertical (lift/lower)
struct Pose {
  int shoulder;
  int elbow;
  int wrist;
};

enum Role { TRIPOD_A, TRIPOD_B };

struct LegPins {
  int shoulder = -1;
  int elbow = -1;
  int wrist = -1;
};

struct Leg {
  String name;
  Role role = TRIPOD_A;
  LegPins pins;
  Servo shoulder, elbow, wrist;
};

const int MAX_LEGS = 6;
const int NUM_STATES = 4;
const unsigned long STATE_MS = 500; // time per state

// ---- move_servo.cpp : talks to the hardware ----
void attachLeg(Leg& leg);
void moveLeg(Leg& leg, const Pose& pose);

// ---- leg_sm.cpp : gait table / which pose for which state ----
void applyLegState(Leg& leg, int state);

// ---- start_legs.cpp : reads config.yaml, creates legs, runs shared clock ----
bool startLegs();
void updateLegs();