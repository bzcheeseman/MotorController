//
// Created by slab on 7/25/2016.
//

#include "Axis.hpp"

/**
 * @file Axis.cpp
 * @copybrief Axis.hpp
 *
 * MotorController - Controlling basic NEMA-17 stepper motors with an arduino.
 * Copyright (C) 2016  Aman LaChapelle
 *
 * Full Notice at MotorController/LICENSE.txt
 */

Axis::Axis(int dirPin, int stepPin, int resetPin, int sleepPin, int stopPin_plus, int stopPin_minus, char id):
        dirPin(dirPin),
        stepPin(stepPin),
        resetPin(resetPin),
        sleepPin(sleepPin),
        stopPin_plus(stopPin_plus),
        stopPin_minus(stopPin_minus),
        id(id) {

    pinMode(this->stepPin, OUTPUT);
    pinMode(this->dirPin, OUTPUT);
    pinMode(this->resetPin, OUTPUT);
    pinMode(this->sleepPin, INPUT);
    pinMode(this->stopPin_plus, INPUT);
    pinMode(this->stopPin_minus, INPUT);
    pinMode(13, OUTPUT);

    Serial.print("Motor ");
    Serial.print(id);
    Serial.println(" Initialized");

    calibrated = false;
}

Axis::~Axis(){
    Serial.print("Motor ");
    Serial.print(this->id);
    Serial.println(" going offline.");
}

void Axis::_plus(torqueMode_t torque_mode){
    // Set the dir pin to high as a reference
    digitalWrite(dirPin, HIGH);

    //High torque mode is just more time between steps of the pins.
    if (torque_mode == HIGH_T){
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(5000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(5000);
    }
    else if (torque_mode == MED_T){
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2000);
    }
    else{
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    return;
}

void Axis::_minus(torqueMode_t torque_mode) {
    // Set the dir pin to high as a reference
    digitalWrite(dirPin, LOW);

    //High torque mode is just more time between steps of the pins.
    if (torque_mode == HIGH_T){
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(5000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(5000);
    }
    else if (torque_mode == MED_T){
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2000);
    }
    else{
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    return;
}

void Axis::Steps(int numSteps, int torque_mode, bool plus){

    int sleep = digitalRead(sleepPin);
    digitalWrite(resetPin, sleep);

    switch (torque_mode){
        case 1: torqueMode = HIGH_T; break;
        case 2: torqueMode = MED_T; break;
        case 3: torqueMode = LOW_T; break;
        default: torqueMode = LOW_T; break;
    }

    if (plus){
        for (int i = 0; i < numSteps; i++){
            this->_plus(torqueMode);
        }
        delay(1000);
    }
    else{
        for (int i = 0; i < numSteps; i++){
            this->_minus(torqueMode);
        }
        delay(1000);
    }

    digitalWrite(resetPin, LOW);

    Serial.println("Moved");

    return;

}

String Axis::moveAlongAxis(int numSteps, int torque_mode, bool plus) {
    if (calibrated){
        ;
    }
    else{
        Serial.println("Uncalibrated!");
        return "LOGGING: ERROR - UNCALIBRATED";
    }

    String log;
    log += "LOGGING: Steps = ";
    log += numSteps;
    log += "\n";

    int sleep = digitalRead(sleepPin);
    digitalWrite(resetPin, sleep);

    switch (torque_mode){
        case 1: torqueMode = HIGH_T; break;
        case 2: torqueMode = MED_T; break;
        case 3: torqueMode = LOW_T; break;
        default: torqueMode = LOW_T; break;
    }

    if (plus){
        for (int i = 0; i < numSteps; i++){
            //Update current position
            current_position++;
            //Test current position, move if valid, don't move if not valid
            if (current_position >= axisLen){
                Serial.println("Positive Limit Reached.  Stopping at ");
                Serial.print(current_position);
                log += "LOGGING: Moved ";
                log += i;
                log += " steps.\nLOGGING: Current position = ";
                log += current_position;
                log += "\n";
                digitalWrite(resetPin, LOW);
                return log;
            }
            else{
                this->_plus(torqueMode);
            }
        }
        //Log outcome
        delayMicroseconds(500);
        log += "LOGGING: Moved ";
        log += numSteps;
        log += " steps.\nLOGGING: Current position = ";
        log += current_position;
        log += "\n";
        digitalWrite(resetPin, LOW);
        return log;
    }
    else{
        for (int i = 0; i < numSteps; i++){
            //Update current position
            current_position--;
            //Test current position, move if valid, don't move if not valid
            if (current_position <= 0){
                Serial.print("Negative Limit Reached.  Stopping at ");
                Serial.println(current_position);
                log += "LOGGING: Moved ";
                log += i;
                log += " steps.\nLOGGING: Current position = ";
                log += current_position;
                log += "\n";
                digitalWrite(resetPin, LOW);
                return log;
            }
            else{
                this->_minus(torqueMode);
            }
        }
        //Log outcome
        delayMicroseconds(500);
        log += "LOGGING: Moved ";
        log += numSteps;
        log += " steps.\nLOGGING: Current position = ";
        log += current_position;
        log += "\n";
        digitalWrite(resetPin, LOW);
        return log;
    }
}

int Axis::getCurrentPosition() {
    return current_position;
}

void Axis::calibrateAxis(float dist_cm) {
    digitalWrite(13, HIGH);
    //minus direction - high means it hit the end stop
    while (HIGH != digitalRead(stopPin_minus)){
        this->_minus(LOW_T);
    }
    current_position = 0;

    //Now run it to the plus direction
    while (HIGH != digitalRead(stopPin_plus)){
        current_position++;
        this->_plus(LOW_T);
    }
    //set the length of the axis for future calibration stuff
    axisLen = current_position;

    distPerStep = dist_cm/float(current_position);
    digitalWrite(13, LOW);
    calibrated = true;
}

String Axis::moveDistance(float dist_cm, int torque_mode, bool plus) {
    if (calibrated){
        int steps = round(dist_cm * distPerStep);
        String output = moveAlongAxis(steps, torque_mode, plus);
        return output;
    }
    else{
        Serial.println("Uncalibrated!");
        return "LOGGING: ERROR - UNCALIBRATED";
    }
}

String Axis::Home(int torque_mode) {

    if (calibrated){
        switch (torque_mode){
            case 1: torqueMode = HIGH_T; break;
            case 2: torqueMode = MED_T; break;
            case 3: torqueMode = LOW_T; break;
            default: torqueMode = LOW_T; break;
        }

        for (int i = 0; i < current_position; i++){
            //Update current position
            current_position--;
            this->_minus(torqueMode);
        }
    }
    else{
        Serial.println("Uncalibrated!");
        return "ERROR - UNCALIBRATED";
    }


}
