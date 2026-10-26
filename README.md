SCORPION STATE MACHINE!!!!111!

How it works:

1. Startup:
    Once the esp is started up, we need to initialize all of the servos. 
    These servos will go ahead and read from which gpio pins they are connected to on the esp.
    This is done through a config.yaml file which is where we declare all of the legs, their pins and their tripods.
    For each servo to know what leg they are a part of, they all will connect in this step to join as one leg
    Once initialized they will all start at their start index in the state machine to start.


2. Movement (state machine):
    Each of these servos are moved using a custom state machine. Here we will declare which servos we want to move, how we want them to move, and how we plan on them moving.
    Since we are limited to 180deg range servos we have to make sure to keep their values they can move within these bounds.
    State machine goes as such:
        TRIPOD A (front right leg, middle left left, back right leg):
            - LIFT_UP
            - SWING_FORWARD
            - LOWER_DOWN 
            - PUSH_BACK
        TRIPOD B (front left leg, middle right left, back left leg):
            - PUSH_BACK
            - PUSH_BACK_END
            - LIFT_UP
            - SWING_AND_LOWER

WE HAVE A AND B TRIPOD BECAUSE:

When the scorpion is walking the scorpion will be on 3 legs at a time. 
Start moving forward -> left leg front, right leg middle, left leg back will go up and the other three will stay down.
Now we have to make the scorpion balance on these 3 legs. 
After the those 3 legs touch back down, then we want the next three legs to go up to imitate walking.

Same thing for going backwards


So far the states that are allowed:
- Walking forward

TODO:
- correction math based on if the leg doesn't go the full distance or robot falls on one side (plan is to use ESP's giro and other telemetry data)