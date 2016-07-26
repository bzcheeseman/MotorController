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

void setup() {
    Serial.begin(9600);
}

void loop() {

    String numSteps;

    while (Serial.available()){
        delay(3);
        if (Serial.available()){
            char c = Serial.read();
            numSteps += c;
        }
    }

    if (numSteps.length() > 0) {
        Serial.println(numSteps);
        if (numSteps.substring(0, 1) == "-") {
            Serial.println("Got a minus - using minus");
//            digitalWrite(52, HIGH);
            int steps = numSteps.substring(1).toInt();
            stepper.moveAlongAxis(steps, 3, false);
//            digitalWrite(52, LOW);
        }
        else {
            Serial.println("Didn't get a minus - using plus");
//            digitalWrite(52, HIGH);
            int steps = numSteps.toInt();
            stepper.moveAlongAxis(steps, 3, true);
//            digitalWrite(52, LOW);

        }

        numSteps = "";
    }
}