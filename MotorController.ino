#include <Arduino.h>
#include <HardwareSerial.h>
#include "Axis/Axis.hpp"

/**
 * @file MotorController.ino
 * @brief The Arudino file
 *
 * MotorController - Controlling basic NEMA-17 stepper motors with an arduino.
 * Copyright (C) 2016  Aman LaChapelle
 *
 * Full Notice at MotorController/LICENSE.txt
 */

Axis stepper(3, 4, 52, 53, 50, 51, 'Y');

void setup() {
    Serial.begin(9600);
}

void loop() {

    String command, log;

    while (Serial.available()){
        delay(3);
        if (Serial.available() > 0){
            command = Serial.readStringUntil('\0');
        }
    }

    if (command.length() > 0){
        command.toLowerCase();
        if (command.substring(0, 9) == "calibrate"){
            log += "LOGGING: Got calibrate command\n";
            float axislen = command.substring(10).toFloat();
            stepper.calibrateAxis(axislen);
        }
        else if (command.substring(0, 4) == "step"){
            log += "LOGGING: Got step command\n";
            String num_steps = command.substring(5);
            if (num_steps[0] == '-'){
                log += "LOGGING: Got a minus - using minus\n";
                int steps = num_steps.substring(1).toInt();
                log += stepper.moveAlongAxis(steps, 3, false);
            }
            else{
                log += "LOGGING: Didn't get a minus - using plus\n";
                int steps = num_steps.toInt();
                log += stepper.moveAlongAxis(steps, 3, true);
            }
        }
        else if (command.substring(0, 8) == "distance"){
            log += "LOGGING: Got distance command\n";
            float dist = command.substring(9).toFloat();
            if (dist > 0){
                log += stepper.moveDistance(dist, 3, true);
            }
            else{
                log += stepper.moveDistance(abs(dist), 3, false);
            }
        }
        else if (command.substring(0, 3) == "home"){
            log += "LOGGING: Got home command\n";
            log += stepper.Home(3);
        }
        else if (command.substring(0, 8) == "position"){
            log += "LOGGING: Got position command\nCurrent position: ";
            Serial.print("Current position: ");
            Serial.println(stepper.getCurrentPosition());
            log += stepper.getCurrentPosition();
        }
        else if (command.substring(0, 5) == "debug"){
            log += "LOGGING: Got debug command - turning off safety checks\n";
            String num_steps = command.substring(6);
            if (num_steps[0] == '-'){
                log += "LOGGING: Got a minus - using minus\n";
                int steps = num_steps.substring(1).toInt();
                Serial.println(steps);
                stepper.Steps(abs(steps), 3, false);
            }
            else{
                log += "LOGGING: Didn't get a minus - using plus\n";
                int steps = num_steps.toInt();
                stepper.Steps(steps, 3, true);
            }
        }
        else{
            log += "LOGGING: Unknown command\n";
            Serial.println("Unknown or Unimplemented command!\n "
                                   "Commands include: {'calibrate', 'step', 'home', 'position', 'debug' <use sparingly and with caution> }");
        }
        Serial.println(log);
        command = "";
        log = "";
    }

//    String numSteps;
//
//    while (Serial.available()) {
//        delay(3);
//        if (Serial.available() > 0) {
//            char c = Serial.read();
//            numSteps += c;
//        }
//    }
//
//    if (numSteps.length() > 0) {
//        Serial.print(numSteps);
//        Serial.println(" steps");
//
//        String log;
//        Serial.println(numSteps);
//        if (numSteps.substring(0, 1) == "-") {
//            log += "Got a minus - using minus\n";
//            int steps = numSteps.substring(1).toInt();
//            log += stepper.moveAlongAxis(steps, 3, false);
//        }
//        else {
//            log += "Didn't get a minus - using plus\n";
//            int steps = numSteps.toInt();
//            log += stepper.moveAlongAxis(steps, 3, true);
//
//        }
//        Serial.println("LOGGING");
//        Serial.println(log);
//        numSteps = "";
//    }


}