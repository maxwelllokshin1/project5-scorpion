#include "leg.h"

enum Action {
  LIFT_UP,         // raise leg vertically (no shoulder change)
  SWING_FORWARD,   // move shoulder forward while in air
  LOWER_DOWN,      // plant foot on ground
  PUSH_BACK,       // start driving (body moves forward)
  PUSH_BACK_END,   // finish driving
  SWING_AND_LOWER, // swing forward and plant
  ACTION_COUNT
};

// TODO: tune these on the real leg
const Pose POSES[ACTION_COUNT] = {
  {90, 60, 60},   // LIFT_UP
  {130, 60, 60},  // SWING_FORWARD
  {130, 90, 90},  // LOWER_DOWN
  {100, 90, 90},  // PUSH_BACK
  {50, 90, 90},   // PUSH_BACK_END
  {130, 90, 90},  // SWING_AND_LOWER
};

const Action SCHEDULE[2][NUM_STATES] = {
  {LIFT_UP, SWING_FORWARD, LOWER_DOWN, PUSH_BACK}, // TRIPOD A
  {PUSH_BACK, PUSH_BACK_END, LIFT_UP, SWING_AND_LOWER}, // TRIPOD B
};


void applyLegState(Leg& leg, int state) { // move the leg based on the state
  moveLeg(leg, POSES[SCHEDULE[leg.role][state % NUM_STATES]]);
}