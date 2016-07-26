//
// Created by slab on 7/25/2016.
//

#ifndef MOTORCONTROLLER_AXIS_HPP
#define MOTORCONTROLLER_AXIS_HPP

#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>

/*
 * TODO: Implement logging somehow
 * TODO: Implement Axis calibration - we have positions correlated to steps now, just gotta calibrate it.
 * TODO: Fix the documentation to be useful in Axis files and MotorController.ino
 * TODO: Fix doxygen output to actually show my docs
 */

/**
 * @file Axis.hpp
 * @brief Holds the declarations for the Axis class, which controls motor movement along one axis.
 *
 * MotorController - Controlling basic NEMA-17 stepper motors with an arduino.
 * Copyright (C) 2016  Aman LaChapelle
 *
 * Full Notice at MotorController/LICENSE.txt
 */

class Axis {

private:
    //! The length of the axis
    int axisLen;

    //! Holds the current position of the stage
    int current_position;
public:
    /**
     * Class constructor - sets up the pins properly and a short message alerting the user that the motor was
     *                     properly initialized.
     *
     * Sets the axis parameter such that the first entry and the last entry are ones, the rest are zeros.  We
     * determine the end by ANDing a 1 with the current position and if the AND returns 1 then we know to end.
     *
     * Assumes we start at position 0.
     *
     * @param axisLen Total number of steps along the axis
     * @param dirPin Reference pin, determines which direction the stepper steps.  This is dependent on how the motor
     *               is connected to the driver.
     * @param stepPin Cycle this pin to step the motor
     * @param resetPin If this pin is different from sleepPin the motor is in sleep mode.  If they are the same,
     *                 (AKA both HIGH) then the motor will turn on and step actively
     * @param sleepPin This pin is set by the board and we read it in to know how to set resetPin.
     * @param id An identifier for each motor.  Easiest if it corresponds to the motor's axis of control.
     * @return void
     */

    Axis(int axisLen, int dirPin, int stepPin, int resetPin, int sleepPin, char id);

    /**
     * Class destructor
     *
     * Deletes the axis array.
     */
    ~Axis();

private:
    //! Need two pins for this, step occurs when we take one from high to low.
    const unsigned int dirPin;
    const unsigned int stepPin;

    //! Sleep and reset pins - drive reset high to bring the board open
    const unsigned int resetPin;
    const unsigned int sleepPin;

    //! Sets the id of the stepper - x, y, z, etc.
    const char id;
    //! Selects the mode of the motor - high torque, medium or low
    enum torqueMode_t {
        HIGH_T,
        MED_T,
        LOW_T
    };

    //! Torque mode of the motor, one of the items in the enum torqueMode_t
    torqueMode_t torqueMode;

    /**
     * Individual method for movement in the plus/minus direction
     *
     * @param torque_mode High, Medium, or Low torque.  Private members only accessible through Steps(int,int,bool).
     * @see Steps(int,int,bool)
     */
    void _plus(torqueMode_t torque_mode);

    /**
     * @copydoc _plus(torqueMode_t)
     */
    void _minus(torqueMode_t torque_mode);
public:
    /**
     * Simple interface for taking some nubmer of steps in either the plus or minus direction.  Specific to each stepper
     * so that we don't need to specify which axis here.
     *
     * @param numSteps The number of steps to take
     * @param torqueMode 1 = HIGH_T, 2 = MED_T, 3 = LOW_T - these are set by the timing of the pin switching.
     * @param plus If true, then we use the _plus(torqueMode_t) routine, if false, we use the _minus(torqueMode_t) routine.
     */
    void Steps(int numSteps, int torqueMode, bool plus);

    /**
     * Tells the motor to move a certain distance.  Assumes we start at zero.
     *
     * @param steps the number of steps we want to move (ideally)
     */
    void moveAlongAxis(int steps, int torqueMode, bool plus);

    /**
     * Gets the current position of the motor.
     *
     * @return current_position
     */
    int getCurrentPosition();
};


#endif //MOTORCONTROLLER_AXIS_HPP
