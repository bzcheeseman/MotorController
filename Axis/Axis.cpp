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
    pinMode(this->stopPin_plus - 1, OUTPUT);
    pinMode(this->stopPin_plus, INPUT);
    pinMode(this->stopPin_minus - 1, OUTPUT);
    pinMode(this->stopPin_minus, INPUT);
    pinMode(13, OUTPUT);

    this->torqueMode = LOW_T;
}

Axis::~Axis(){
    ;
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

    Serial.print("Moved ");
    Serial.println(numSteps);
    Serial.println("");

    return;

}

String Axis::moveAlongAxis(bool plus, passData& info) {
    if (info.calibrated){
        ;
    }
    else{
        Serial.println("Uncalibrated!");
        return "LOGGING: ERROR - UNCALIBRATED";
    }

    String log;
    log += "LOGGING: Steps = ";
    log += info.numSteps;
    log += "\n";

    int sleep = digitalRead(sleepPin);
    digitalWrite(resetPin, sleep);

    switch (info.torque_mode){
        case 1: torqueMode = HIGH_T; break;
        case 2: torqueMode = MED_T; break;
        case 3: torqueMode = LOW_T; break;
        default: torqueMode = LOW_T; break;
    }

    if (plus){
        for (int i = 0; i < info.numSteps; i++){
            //Update current position
            info.currentPosition++;
            //Test current position, move if valid, don't move if not valid
            if (info.currentPosition >= info.axisLen){
                Serial.print("Positive Limit Reached.  Stopping at ");
                Serial.println(info.currentPosition);
                log += "LOGGING: Moved ";
                log += i;
                log += " steps.\nLOGGING: Current position = ";
                log += info.currentPosition;
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
        log += info.numSteps;
        log += " steps.\nLOGGING: Current position = ";
        log += info.currentPosition;
        log += "\n";
        digitalWrite(resetPin, LOW);
        return log;
    }
    else{
        for (int i = 0; i < info.numSteps; i++){
            //Update current position
            info.currentPosition--;
            //Test current position, move if valid, don't move if not valid
            if (info.currentPosition <= 0){
                Serial.print("Negative Limit Reached.  Stopping at ");
                Serial.println(info.currentPosition);
                log += "LOGGING: Moved ";
                log += i;
                log += " steps.\nLOGGING: Current position = ";
                log += info.currentPosition;
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
        log += info.numSteps;
        log += " steps.\nLOGGING: Current position = ";
        log += info.currentPosition;
        log += "\n";
        digitalWrite(resetPin, LOW);
        return log;
    }
}

char Axis::getID() {
    return id;
}


void Axis::calibrateAxis(passData& info) {

    int sleep = digitalRead(sleepPin);
    digitalWrite(resetPin, sleep);

    digitalWrite(this->stopPin_minus - 1, 1);
    delayMicroseconds(10);
    if (digitalRead(this->stopPin_minus) != 0){
        Serial.println("Some error in the wiring occurred!");
        Serial.println("LOGGING: CALIBRATION ERROR - ENDSTOP TRIGGERED PREMATURELY");
        digitalWrite(resetPin, LOW);
        return;
    }
    //minus direction - high means it hit the end stop
    while (1){
        delay(10);
        int stop = digitalRead(stopPin_minus);
        if (stop != 0){
            Serial.println("LOGGING: Minus triggered");
            break;
        }
        this->_minus(torqueMode);
    }
    info.currentPosition = 0;
    delay(100);

    digitalWrite(this->stopPin_minus - 1, 0);
    digitalWrite(this->stopPin_plus - 1, 1);
    delayMicroseconds(10);
    if (digitalRead(this->stopPin_plus) != 0){
        Serial.println("Some error in the wiring occurred!");
        Serial.println("LOGGING: CALIBRATION ERROR - ENDSTOP TRIGGERED PREMATURELY");
        digitalWrite(resetPin, LOW);
        return;
    }
    //Now run it to the plus direction
    while (1){
        delay(20);
        int stop = digitalRead(stopPin_plus);
        if (stop != 0){
            Serial.println("LOGGING: Plus triggered");
            break;
        }
        info.currentPosition++;
        this->_plus(torqueMode);
    }

    //set the length of the axis for future calibration stuff
    info.axisLen = info.currentPosition;

//    for (int i = 0; i < 100; i++){
//        info.currentPosition--;
//        this->_minus(torqueMode);
//    }

    Serial.print("LOGGING: Current position: ");
    Serial.println(info.currentPosition);

    info.distPerStep = info.dist_cm/float(info.currentPosition);
    digitalWrite(this->stopPin_plus-1, 0);
    digitalWrite(resetPin, LOW);
    info.calibrated = true;
}

String Axis::moveDistance(bool plus, passData& info) {
    if (info.calibrated){
        info.numSteps = round(info.dist_cm/info.distPerStep);
        String output = moveAlongAxis(plus, info);
        return output;
    }
    else{
        Serial.println("Uncalibrated!");
        return "LOGGING: ERROR - UNCALIBRATED";
    }
}

String Axis::Home(passData& info) {

    if (info.calibrated){

        this->Steps(info.currentPosition, info.torque_mode, false);
        info.currentPosition = 0;
        return "Axis homed.  Current position: " + String(info.currentPosition);

    }
    else{
        Serial.println("Uncalibrated!");
        return "ERROR - UNCALIBRATED";
    }


}






