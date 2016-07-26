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

Axis stepper(1450, 3, 4, 52, 53, 'Y');
bool calibrated;

void setup() {
    Serial.begin(9600);
}

void loop() {

    String command;
    while (Serial.available()){
        delay(3);
        if (Serial.available()){
            char c = Serial.read();
            command += c;
        }
    }

    if (command.length() > 0){
        if (command == "calibrate" or command == "CALIBRATE" or command == "Calibrate"){
            calibrated = true;
            stepper.Home(3)
            stepper.Steps(100 ,3, true);
            Serial.println("Enter distance moved in cm");
            String distance;
            while (Serial.available()){
                delay(3);
                if (Serial.available()){
                    char d = Serial.read();
                    distance += d;
                }
            }
            stepper.calibrateAxis(distance.toFloat());
        }
        else if (command == "step" or command == "STEP" or command == "Step"){
            String numSteps;

            while (Serial.available()){
                delay(3);
                if (Serial.available()){
                    char c = Serial.read();
                    numSteps += c;
                }
            }

            if (numSteps.length() > 0) {
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
        }
        else if (command == "distance" or command == "DISTANCE" or command == "Distance"){
            String travel;

            while (Serial.available()){
                delay(3);
                if (Serial.available()){
                    char c = Serial.read();
                    travel += c;
                }
            }

            if (travel.length() > 0) {
                String log;
                Serial.println(travel);
                if (travel.substring(0, 1) == "-") {
                    log += "Got a minus - using minus\n";
                    int dist = travel.substring(1).toFloat();
                    log += stepper.moveDistance(dist, 3, false, calibrated);
                }
                else {
                    log += "Didn't get a minus - using plus\n";
                    int dist = travel.toFloat();
                    log += stepper.moveDistance(dist, 3, true, calibrated);

                }
                Serial.println("LOGGING");
                Serial.println(log);
                travel = "";
            }
        }
    }

}