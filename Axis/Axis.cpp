//
// Created by slab on 7/25/2016.
//

#include "Axis.hpp"

/**
 * @file Axis.cpp
 * @copybrief Axis.hpp
 *
 * other stuff here
 */

Axis::Axis(int axisLen, int dirPin, int stepPin, int resetPin, int sleepPin, char id):
        axisLen(axisLen),
        dirPin(dirPin),
        stepPin(stepPin),
        resetPin(resetPin),
        sleepPin(sleepPin),
        id(id) {

    pinMode(this->stepPin, OUTPUT);
    pinMode(this->dirPin, OUTPUT);
    pinMode(this->resetPin, OUTPUT);
    pinMode(this->sleepPin, INPUT);

    Serial.print("Motor ");
    Serial.print(id);
    Serial.println(" Initialized");

    current_position = 0;
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

void Axis::moveAlongAxis(int numSteps, int torque_mode, bool plus) {

    String log;
    log += "Steps = ";
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
                log += "Moved ";
                log += i;
                log += " steps.\nCurrent position = ";
                log += current_position;
                log += "\n";
                break;
            }
            else{
                this->_plus(torqueMode);
            }
        }
        //Log outcome
        delayMicroseconds(500);
        log += "Moved ";
        log += numSteps;
        log += " steps.\nCurrent position = ";
        log += current_position;
        log += "\n";
        return;
    }
    else{
        for (int i = 0; i < numSteps; i++){
            //Update current position
            current_position--;
            //Test current position, move if valid, don't move if not valid
            if (current_position <= 0){
                Serial.print("Negative Limit Reached.  Stopping at ");
                Serial.println(current_position);
                log += "Moved ";
                log += i;
                log += " steps.\nCurrent position = ";
                log += current_position;
                log += "\n";
                break;
            }
            else{
                this->_minus(torqueMode);
            }
        }
        //Log outcome
        delayMicroseconds(500);
        log += "Moved ";
        log += numSteps;
        log += " steps.\nCurrent position = ";
        log += current_position;
        log += "\n";
        return;
    }
}

int Axis::getCurrentPosition() {
    return current_position;
}
