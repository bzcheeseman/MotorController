#include <Arduino.h>
#include <HardwareSerial.h>
#include "Axis/Axis.hpp"
#include <AccelStepper.h>

/**
 * @file MotorController.ino
 * @brief The Arudino file
 *
 * MotorController - Controlling basic NEMA-17 stepper motors with an arduino.
 *
 * Copyright (C) 2016  Aman LaChapelle
 *
 * Full Notice at MotorController/LICENSE.txt
 */

//for some reason it's not keeping the current position outside the function...need to fix that

Axis steppers[3] = {Axis (8, 9, 52, 53, 23, 25, 'x'),
                    Axis (5, 6, 50, 51, 27, 29, 'y'),
                    Axis (3, 4, 48, 49, 26, 27, 'z')};

passData info[3];

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

passData &chooseInfo(char which){
    if (which == 'x'){
        return info[0];
    }
    else if (which == 'y'){
        return info[1];
    }
    else if (which == 'z'){
        return info[2];
    }
    else{
        Serial.println("No such axis!");
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
//        Serial.println(command);
        command.toLowerCase();
        if (command.substring(0, 4) == "who"){
            Serial.println("MotorController");
            Serial.print("Axes ");
            for (int i = 0; i < 3; i++){
                Serial.print(steppers[i].getID());
                Serial.print(" ");
            }
            Serial.println("initialized");
        }
        else if (command.substring(0, 10) == "calibrated"){
            Serial.println(chooseInfo(command[11]).calibrated);
        }
        else if (command.substring(0, 9) == "calibrate"){
            Serial.print("LOGGING: Got calibrate command for axis ");
            Serial.println(command[10]);
            float axislen = command.substring(12).toFloat();
            Axis which (chooseStepper(command[10]));
            chooseInfo(command[10]).dist_cm = axislen;
            chooseInfo(command[10]).torque_mode = 3;
            which.calibrateAxis(chooseInfo(command[10]));
        }
        else if (command.substring(0, 4) == "step"){
            Serial.print("LOGGING: Got step command for axis ");
            Serial.println(command[5]);
            Axis which (chooseStepper(command[5]));
            chooseInfo(command[5]).torque_mode = 3;
            String num_steps = command.substring(7);
            if (num_steps[0] == '-'){
                Serial.println("LOGGING: Got a minus - using minus");
                chooseInfo(command[5]).numSteps = abs(num_steps.toInt());
                Serial.println(which.moveAlongAxis(false, chooseInfo(command[5])));
            }
            else{
                Serial.println("LOGGING: Didn't get a minus - using plus");
                chooseInfo(command[5]).numSteps = num_steps.toInt();
                Serial.println(which.moveAlongAxis(true, chooseInfo(command[5])));
            }
        }
        else if (command.substring(0, 8) == "distance"){
            Serial.print("LOGGING: Got distance command for axis ");
            Serial.println(command[9]);
            Axis which (chooseStepper(command[9]));
            chooseInfo(command[9]).torque_mode = 3;
            chooseInfo(command[9]).dist_cm = command.substring(11).toFloat();
            if (chooseInfo(command[9]).dist_cm > 0){
                Serial.println(which.moveDistance(true, chooseInfo(command[9])));
            }
            else{
                chooseInfo(command[9]).dist_cm = abs(command.substring(11).toFloat());
                Serial.println(which.moveDistance(false, chooseInfo(command[9])));
            }
        }
        else if (command.substring(0, 4) == "home"){
            Serial.print("LOGGING: Got home command for axis ");
            Serial.println(command[5]);
            Axis which (chooseStepper(command[5]));
            chooseInfo(command[5]).torque_mode = 3;
            Serial.println(which.Home(chooseInfo(command[5])));
        }
        else if (command.substring(0, 8) == "position"){
            Serial.print("LOGGING: Got position command for axis ");
            Serial.println(command[9]);
            Serial.print("Current position: ");
            Serial.println(chooseInfo(command[9]).currentPosition);
        }
        else if (command.substring(0, 5) == "debug"){
            Serial.print("LOGGING: Got debug command - turning off safety checks for axis ");
            Serial.println(command[6]);
            Axis which (chooseStepper(command[6]));
            chooseInfo(command[6]).torque_mode = 3;
            String num_steps = command.substring(8);
            if (num_steps[0] == '-'){
                Serial.println("LOGGING: Got a minus - using minus");
                chooseInfo(command[6]).numSteps = num_steps.toInt();
                which.Steps(abs(chooseInfo(command[6]).numSteps), chooseInfo(command[6]).torque_mode, false);
            }
            else{
                Serial.println("LOGGING: Didn't get a minus - using plus");
                chooseInfo(command[6]).numSteps = num_steps.toInt();
                which.Steps(chooseInfo(command[6]).numSteps, chooseInfo(command[6]).torque_mode, true);
            }
        }
        else{
            Serial.println("LOGGING: Unknown command");
            Serial.println("Unknown or Unimplemented command!\n "
                                   "Commands include: {'who', 'calibrate', 'distance', 'step', 'home', 'position', 'debug' <use sparingly and with caution> }");
        }


        command = "";
    }
}
