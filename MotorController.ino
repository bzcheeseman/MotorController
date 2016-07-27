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

//    String command;
//    while (Serial.available()) {
//        delay(3);
//        if (Serial.available() > 0) {
//            char c = Serial.read();
//            command += c;
//        }
//    }
//
//    if (command.length() > 0){
//        Serial.println(command);
//        if (command == "steps"){
//            Serial.println("Number of steps: ");
//            String num;
//            while (Serial.available() > 0) {
//                char c = Serial.read();
//                Serial.print(c);
//                num += c;
//            }
//            if (num.length() > 0){
//                Serial.print("Steps within the command thingy ");
//                Serial.println(num);
//            }
//            num = "";
//        }
//        command = "";
//    }

    String numSteps;

    while (Serial.available()) {
        delay(3);
        if (Serial.available() > 0) {
            char c = Serial.read();
            numSteps += c;
        }
    }

    if (numSteps.length() > 0) {
        Serial.print(numSteps);
        Serial.println(" steps");

        String log;
        Serial.println(numSteps);
        if (numSteps.substring(0, 1) == "-") {
            log += "Got a minus - using minus\n";
            int steps = numSteps.substring(1).toInt();
            log += stepper.moveAlongAxis(steps, 3, false);
        }
        else {
            log += "Didn't get a minus - using plus\n";
            int steps = numSteps.toInt();
            log += stepper.moveAlongAxis(steps, 3, true);

        }
        Serial.println("LOGGING");
        Serial.println(log);
        numSteps = "";
    }








//    String command;
//    while (Serial.available() > 0) {
//        char c = Serial.read();
//        if (c == ';'){
//            break;
//        }
//        else{
//            command += c;
//        }
//
//    }
//
//    if (command.length() > 0) {
//        Serial.println(command);
//
//        if (command == "calibrate" or command == "CALIBRATE" or command == "Calibrate") {
//
//            Serial.println("Calibrating");
//            stepper.Home(3);
//            stepper.Steps(100, 3, true);
//            Serial.println("Enter distance moved in cm");
//            String distance;
//            int av = Serial.available();
//            Serial.println(av);
//            if (Serial.available() > 0) {
//                char d = Serial.read();
//                distance += d;
//                Serial.print(distance);
//            }
//            if (distance.length() > 0) {
//                stepper.calibrateAxis(distance.toFloat());
//                calibrated = true;
//            }
//            distance = "";
//        }
//        else if (command == "step" or command == "STEP" or command == "Step") { //add print statements
//            Serial.println("Enter the number of steps to move");
//
//            String numSteps;
//
//            while (Serial.available()) {
//                delay(3);
//                if (Serial.available() > 0) {
//                    char c = Serial.read();
//                    numSteps += c;
//                    Serial.print(numSteps);
//                }
//
//                if (numSteps.length() > 0) {
//                    Serial.print(numSteps);
//                    Serial.println(" steps");
//
//                    String log;
//                    Serial.println(numSteps);
//                    if (numSteps.substring(0, 1) == "-") {
//                        log += "Got a minus - using minus\n";
//                        int steps = numSteps.substring(1).toInt();
//                        log += stepper.moveAlongAxis(steps, 3, false);
//                    }
//                    else {
//                        log += "Didn't get a minus - using plus\n";
//                        int steps = numSteps.toInt();
//                        log += stepper.moveAlongAxis(steps, 3, true);
//
//                    }
//                    Serial.println("LOGGING");
//                    Serial.println(log);
//                    numSteps = "";
//                }
//            }
//        }
//        else if (command == "distance" or command == "DISTANCE" or command == "Distance") { //add print statements
//            Serial.println("Enter distance to move in cm");
//
//            String travel;
//
//            while (Serial.available()) {
//                delay(3);
//                if (Serial.available() > 0) {
//                    char c = Serial.read();
//                    travel += c;
//                    Serial.print(travel);
//                }
//
//                if (travel.length() > 0) {
//                    Serial.print(travel);
//                    Serial.println(" cm");
//
//                    String log;
//                    Serial.println(travel);
//                    if (travel.substring(0, 1) == "-") {
//                        log += "Got a minus - using minus\n";
//                        int dist = travel.substring(1).toFloat();
//                        log += stepper.moveDistance(dist, 3, false, calibrated);
//                    }
//                    else {
//                        log += "Didn't get a minus - using plus\n";
//                        int dist = travel.toFloat();
//                        log += stepper.moveDistance(dist, 3, true, calibrated);
//
//                    }
//                    Serial.println("LOGGING");
//                    Serial.println(log);
//                    travel = "";
//                }
//            }
//        }
//    }
//    command = "";

}