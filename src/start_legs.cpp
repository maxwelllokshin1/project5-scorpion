#include "leg.h"
#include <LittleFS.h> // This is meant for internal flash memory

static Leg legs[MAX_LEGS];
static int leg_count = 0;

static int current_state = 0;
static unsigned long state_starts = 0;

// Every top-level key starts a new leg; indented "key: value" lines fill it in.
static bool loadConfig(const char* path) {
  File f = LittleFS.open(path, "r"); // load the path on the hardrive of esp
  
  if (!f) {
    Serial.printf("Could not open %s (did you run 'Upload Filesystem Image'?)\n", path);
    return false;
  }

  Leg* cur = nullptr;

  while (f.available()) { // while the file is still there
    String line = f.readStringUntil('\n'); // read line by line

    // only keep real info not comments
    int hash = line.indexOf('#');
    if (hash >= 0) line.remove(hash); // strip comments

    bool indented = line.length() > 0 && (line[0] == ' ' || line[0] == '\t'); // if the lines are indented: true / false
    line.trim(); // also removes \r
    if (line.length() == 0) continue; // nothing on that line ignore

    // read after the colon
    int colon = line.indexOf(':');
    if (colon < 0) continue;

    String key = line.substring(0, colon); // the name of var
    String val = line.substring(colon + 1); // value of var
    key.trim();
    val.trim();

    if (!indented) { // if you didn't read a leg
      if (leg_count >= MAX_LEGS) { // leg count exceeds the number of expected legs
        Serial.println("Too many legs in config, ignoring the rest");
        break;
      }

      cur = &legs[leg_count++]; // go to next leg
      cur->name = key;
    } else if (cur) { // if you did read a leg now get joint info
      if (key == "shoulder") cur->pins.shoulder = val.toInt(); 
      else if (key == "elbow") cur->pins.elbow = val.toInt(); 
      else if (key == "wrist") cur->pins.wrist = val.toInt(); 
      else if (key == "role") cur->role = (val == "B" || val == "b") ? TRIPOD_B : TRIPOD_A;
    }
  }

  f.close(); // close the file

  // validate
  for (int i = 0; i < leg_count; i++) { // for the legs
    const LegPins& p = legs[i].pins;
    if (p.shoulder < 0 || p.elbow < 0 || p.wrist < 0) { // no pin value for any joint
      Serial.printf("REQUIRED KEY NOT FOUND for %s (need shoulder, elbow, wrist)\n", legs[i].name.c_str());
      return false;
    }
  }
  return leg_count > 0; // return true or false if the legs were read
}

// Read from config.yaml and start all servos at 0
bool startLegs() {
  if (!LittleFS.begin(false)) { // check if we can start the flasher
    Serial.println("LittleFS mount failed");
    return false;
  }

  if (!loadConfig("/config.yaml")) return false; // check if we can load the configs

  for (int i = 0; i < leg_count; i++) { // go for all the legs
    attachLeg(legs[i]); // attach the servos (joints) to the leg
    Serial.printf("Started %s (role %c)\n", legs[i].name.c_str(), legs[i].role == TRIPOD_A ? 'A' : 'B'); // debugging line
  }

  current_state = 0;
  state_starts = millis();
  for (int i = 0; i < leg_count; i++)  // for all legs 
  {
    applyLegState(legs[i], current_state); // start at start index
  }
  return true;
}

// One shared clock steps every leg together so the tripods stay in sync.
void updateLegs() {
  if (leg_count == 0) return; // no legs
  if (millis() - state_starts >= STATE_MS) { // the new state
    current_state = (current_state + 1) % NUM_STATES; // update state
    state_starts = millis();
    for (int i = 0; i < leg_count; i++) applyLegState(legs[i], current_state); 
  }
}