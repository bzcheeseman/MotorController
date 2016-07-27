//
// Created by slab on 7/25/2016.
//

#ifndef MOTORCONTROLLER_AXIS_HPP
#define MOTORCONTROLLER_AXIS_HPP

#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>

/*
 * TODO: Fix the documentation to be useful in Axis files and MotorController.ino
 * TODO: Fix python interface
 * TODO: Hope the current thing works, test it when possible (when endstops come in)
 * TODO: get the endstops and test out the calibration interface
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

    //! Holds the distance per step
    float distPerStep;
public:
    /**
     * Class constructor - sets up the pins properly and a short message alerting the user that the motor was
     *                     properly initialized.
     *
     * Sets the axis parameter such that the first entry and the last entry are ones, the rest are zeros.  We
     * determine the end by ANDing a 1 with the current position and if the AND returns 1 then we know to end.
     *
     * Uses pin 13 as the reference pin for the endstop switches.  We set it to high to calibrate and then turn it off again.
     *
     * @param dirPin Reference pin, determines which direction the stepper steps.  This is dependent on how the motor
     *               is connected to the driver.
     * @param stepPin Cycle this pin to step the motor
     * @param resetPin If this pin is different from sleepPin the motor is in sleep mode.  If they are the same,
     *                 (AKA both HIGH) then the motor will turn on and step actively
     * @param sleepPin This pin is set by the board and we read it in to know how to set resetPin.
     * @param stopPin_plus This pin is an input pin that is read in to find the stop position on the positive end of the axis.
     * @param stopPin_minus This pin is an input pin that is read in to find the stop position on the negative end of the axis.
     * @param id An identifier for each motor.  Easiest if it corresponds to the motor's axis of control.
     * @return void
     */

    Axis(int dirPin, int stepPin, int resetPin, int sleepPin, int stopPin_plus, int stopPin_minus, char id);

    /**
     * Class destructor
     *
     * Deletes the axis array.
     */
    ~Axis();

private:
    //! Need two pins for stepping, dirPin high means one direction, dirPin low means the other.
    const unsigned int dirPin;
    //! Need two pins for stepping, stepPin high then low indicates one step with dirPin indicating direction.
    const unsigned int stepPin;

    //! Reset pin, drive resetPin to the value of sleepPin to put board in sleep mode.
    const unsigned int resetPin;
    //! Read in value of sleepPin and use it to drive resetPin.
    const unsigned int sleepPin;

    //! Pin that reads in the endstop voltage and references it to HIGH on the minus side
    const unsigned int stopPin_minus;
    //! Pin that reads in the endstop voltage and references it to HIGH on the plus side
    const unsigned int stopPin_plus;

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

    //! Since the calibration sets the length of the axis we need to do that first.
    bool calibrated;

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
     * @param torque_mode 1 = HIGH_T, 2 = MED_T, 3 = LOW_T - these are set by the timing of the pin switching.
     * @param plus If true, then we use the _plus(torqueMode_t) routine, if false, we use the _minus(torqueMode_t) routine.
     */
    void Steps(int numSteps, int torque_mode, bool plus);

public:
    /**
     * Tells the motor to move a certain distance.  Assumes we start at zero.
     *
     * @param steps The number of steps we want to move (ideally)
     * @param torque_mode 1 = HIGH_T, 2 = MED_T, 3 = LOW_T - these are set by the timing of the pin switching.
     * @param plus If true, then we use the _plus(torqueMode_t) routine, if false, we use the _minus(torqueMode_t) routine.
     *
     * @returns Logging string that contains everything executed (that was set up).
     */
    String moveAlongAxis(int steps, int torque_mode, bool plus);

    /**
     * Gets the current position of the motor.
     *
     * @return current_position
     */
    int getCurrentPosition();

    /**
     * Takes the motor home (to position zero)
     *
     * @param torque_mode 1 = HIGH_T, 2 = MED_T, 3 = LOW_T - these are set by the timing of the pin switching.
     */
    String Home(int torque_mode);

    /**
     * Allows us to calibrate the axis steps to a distance.  Assumes full step mode.  This WILL depend on which
     * motor is used.  Calibration should be done in cm for completeness.
     *
     * @param dist_cm Length of the axis
     */
    void calibrateAxis(float dist_cm);

    /**
     * Moves the carriage a certain distance according to the given calibration.
     *
     * @param dist_cm Distance to move
     * @param torque_mode 1 = HIGH_T, 2 = MED_T, 3 = LOW_T - these are set by the timing of the pin switching.
     * @param plus Controls the direction.  @see moveAlongAxis(int,int,bool)
     * @return Logging string that describes the actions.  @see moveAlongAxis(int,int,bool)
     */
    String moveDistance(float dist_cm, int torque_mode, bool plus);

};


#endif //MOTORCONTROLLER_AXIS_HPP
