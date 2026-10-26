#include "leg.h"

void attachLeg(Leg& leg) { // attach the joints to the leg
  leg.shoulder.setPeriodHertz(50);
  leg.elbow.setPeriodHertz(50);
  leg.wrist.setPeriodHertz(50);

  // the pins
  leg.shoulder.attach(leg.pins.shoulder, 500, 2500);
  leg.elbow.attach(leg.pins.elbow, 500, 2500);
  leg.wrist.attach(leg.pins.wrist, 500, 2500);
}

void moveLeg(Leg& leg, const Pose& pose) { // move joint based on what degree we write to it 0<= x < 180
  leg.shoulder.write(constrain(pose.shoulder, 0, 180));
  leg.elbow.write(constrain(pose.elbow, 0, 180));
  leg.wrist.write(constrain(pose.wrist, 0, 180));
}