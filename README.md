MotorController

This project was meant to be a simple way to control NEMA-17 stepper motors.  It's not meant to do as much as fancy software
like Marlin, but it should be functional.  The problem it solves is to give a simple way to convert a 3D printer kit into a translation stage
with arbitrary axes.  We will be using up to 5 axes, and as such the code is written to be modular to each axis - that is, it initializes one axis at a time.