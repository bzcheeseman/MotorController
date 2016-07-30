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

struct axis_t {
    const int numSteppers;
    Axis* stepper;
    const char* ID;

    axis_t(Axis* axes, const char* ids, const int numSteppers): stepper(axes),
                                                                  ID(ids),
                                                                  numSteppers(numSteppers){
        ;
    }
    ~axis_t(){
        delete stepper;
        delete ID;
    }

    Axis operator[](char which){
        for (int i = 0; i < numSteppers; i++){
            if (which == ID[i]){
                Serial.println(ID[i]);
                return stepper[i];
            }
            else{
                ;
            }
        }
    }
};

Axis steppers[1] = {Axis (3, 4, 52, 53, 22, 23, 'z')};
char ids[1] = {'z'};
axis_t* motors;

void setup() {
    Serial.begin(9600);
    while (!Serial){
        ;
    }


    motors = new axis_t (steppers, ids, 1);
    delete steppers;
    delete ids;
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
            motors->operator[](command[10]).calibrateAxis(axislen);
        }
        else if (command.substring(0, 4) == "step"){
            Serial.print("LOGGING: Got step command for axis ");
            Serial.println(command[5]);
            String num_steps = command.substring(7);
            if (num_steps[0] == '-'){
                Serial.println("LOGGING: Got a minus - using minus");
                int steps = num_steps.substring(1).toInt();
                Serial.println(motors->operator[](command[5]).moveAlongAxis(steps, 3, false));
            }
            else{
                Serial.println("LOGGING: Didn't get a minus - using plus");
                int steps = num_steps.toInt();
                Serial.println(motors->operator[](command[5]).moveAlongAxis(steps, 3, true));
            }
        }
        else if (command.substring(0, 8) == "distance"){
            Serial.print("LOGGING: Got distance command for axis ");
            Serial.println(command[9]);
            float dist = command.substring(11).toFloat();
            if (dist > 0){
                Serial.println(motors->operator[](command[9]).moveDistance(dist, 3, true));
            }
            else{
                Serial.println(motors->operator[](command[9]).moveDistance(abs(dist), 3, false));
            }
        }
        else if (command.substring(0, 4) == "home"){
            Serial.print("LOGGING: Got home command for axis ");
            Serial.println(command[5]);
            Serial.println(motors->operator[](command[5]).Home(3));
        }
        else if (command.substring(0, 8) == "position"){
            Serial.print("LOGGING: Got position command for axis ");
            Serial.println(command[9]);
            Serial.print("Current position: ");
            Serial.println(motors->operator[](command[9]).getCurrentPosition());
        }
        else if (command.substring(0, 5) == "debug"){
            Serial.print("LOGGING: Got debug command - turning off safety checks for axis ");
            Serial.println(command[6]);
            String num_steps = command.substring(8);
            if (num_steps[0] == '-'){
                Serial.println("LOGGING: Got a minus - using minus");
                int steps = num_steps.substring(1).toInt();
                Serial.println(steps);
                motors->operator[](command[6]).Steps(abs(steps), 3, false);
            }
            else{
                Serial.println("LOGGING: Didn't get a minus - using plus");
                int steps = num_steps.toInt();
                motors->operator[](command[6]).Steps(steps, 3, true);
            }
        }
        else{
            Serial.println("LOGGING: Unknown command");
            Serial.println("Unknown or Unimplemented command!\n "
                                   "Commands include: {'calibrate', 'step', 'home', 'position', 'debug' <use sparingly and with caution> }");
        }
        command = "";
    }
}