#include <Arduino.h>
#include <HardwareSerial.h>
#include "Axis/Axis.hpp"

/**
 * @file MotorController.ino
 * @brief The Arudino file
 *
 * MotorController - Controlling basic NEMA-17 stepper motors with an arduino.
 * Written using RAII-style class allocation within each loop - should free all resources each loop iteration.
 *
 * Copyright (C) 2016  Aman LaChapelle
 *
 * Full Notice at MotorController/LICENSE.txt
 */

//for some reason it's not keeping the current position outside the function...need to fix that

Axis steppers[3] = {Axis (8, 9, 52, 53, 23, 25, 'x'),
                    Axis (5, 6, 50, 51, 24, 25, 'y'),
                    Axis (3, 4, 48, 49, 26, 27, 'z')};

int positions[3] = {0, 0, 0};
bool calibrated[3] = {false, false, false};

Axis chooseStepper(char which){
    if (which == 'x'){
        return steppers[0];
    }
    else if (which == 'y'){
        return steppers[1];
    }
    else if (which == 'z'){
        return steppers[2];
    }
}

int& choosePosition(char which){
    if (which == 'x'){
        return positions[0];
    }
    else if (which == 'y'){
        return positions[1];
    }
    else if (which == 'z'){
        return positions[2];
    }
}

bool& chooseCalib(char which){
    if (which == 'x'){
        return calibrated[0];
    }
    else if (which == 'y'){
        return calibrated[1];
    }
    else if (which == 'z'){
        return calibrated[2];
    }
}

void setup() {
    Serial.begin(9600);
    while (!Serial){
        ;
    }
}

void loop() {

    String command;

    if (Serial.available()){
        command = Serial.readStringUntil('\n');
    }

    if (command.length() > 0){
        Serial.println(command);
        command.toLowerCase();
        if (command.substring(0, 9) == "calibrate"){
            Serial.print("LOGGING: Got calibrate command for axis ");
            Serial.println(command[10]);
            float axislen = command.substring(12).toFloat();
            Axis which (chooseStepper(command[10]));
            which.calibrateAxis(axislen, choosePosition(command[10]), chooseCalib(command[10]));
        }
        else if (command.substring(0, 4) == "step"){
            Serial.print("LOGGING: Got step command for axis ");
            Serial.println(command[5]);
            Axis which (chooseStepper(command[5]));
            String num_steps = command.substring(7);
            if (num_steps[0] == '-'){
                Serial.println("LOGGING: Got a minus - using minus");
                int steps = num_steps.toInt();
                Serial.println(which.moveAlongAxis(abs(steps), 3, false, choosePosition(command[5]), chooseCalib(command[5])));
            }
            else{
                Serial.println("LOGGING: Didn't get a minus - using plus");
                int steps = num_steps.toInt();
                Serial.println(which.moveAlongAxis(steps, 3, true, choosePosition(command[5]), chooseCalib(command[5])));
            }
        }
        else if (command.substring(0, 8) == "distance"){
            Serial.print("LOGGING: Got distance command for axis ");
            Serial.println(command[9]);
            Axis which (chooseStepper(command[10]));
            float dist = command.substring(11).toFloat();
            if (dist > 0){
                Serial.println(which.moveDistance(dist, 3, true, choosePosition(command[9]), chooseCalib(command[9])));
            }
            else{
                Serial.println(which.moveDistance(abs(dist), 3, false, choosePosition(command[9]), chooseCalib(command[9])));
            }
        }
        else if (command.substring(0, 4) == "home"){
            Serial.print("LOGGING: Got home command for axis ");
            Serial.println(command[5]);
            Axis which (chooseStepper(command[5]));
            Serial.println(which.Home(3, choosePosition(command[5]), chooseCalib(command[9])));
        }
        else if (command.substring(0, 8) == "position"){
            Serial.print("LOGGING: Got position command for axis ");
            Serial.println(command[9]);
            Serial.print("Current position: ");
            Serial.println(choosePosition(command[9]));
        }
        else if (command.substring(0, 5) == "debug"){
            Serial.print("LOGGING: Got debug command - turning off safety checks for axis ");
            Serial.println(command[6]);
            Axis which (chooseStepper(command[6]));
            String num_steps = command.substring(8);
            if (num_steps[0] == '-'){
                Serial.println("LOGGING: Got a minus - using minus");
                int steps = num_steps.toInt();
                which.Steps(abs(steps), 3, false);
            }
            else{
                Serial.println("LOGGING: Didn't get a minus - using plus");
                int steps = num_steps.toInt();
                which.Steps(steps, 3, true);
            }
        }
        else{
            Serial.println("LOGGING: Unknown command");
            Serial.println("Unknown or Unimplemented command!\n "
                                   "Commands include: {'calibrate', 'distance', 'step', 'home', 'position', 'debug' <use sparingly and with caution> }");
        }


        command = "";
    }
}