MotorController

This project was meant to be a simple way to control NEMA-17 stepper motors.  It's not meant to do as much as fancy software
like Marlin, but it should be functional.  The problem it solves is to give a simple way to convert a 3D printer kit into a translation stage
with arbitrary axes.  We will be using up to 5 axes, and as such the code is written to be modular to each axis - that 
is, it initializes one axis at a time.

After downloading and unzipping, import into Clion (honestly not sure how to do it otherwise - it was made with
the Clion arduino plugin...)

To make the documentation, change the target to doc from upload and build - it will autogenerate the Doxygen
files.

Dependencies:
```
cmake
arduino
avr-g++
doxygen
```

NOTE: This project is basically moot at this point - it was created as 
a possible alternative to Marlin because we couldn't get it to work
but we were able to fix it up and so I'm going to call this project
for now, and maybe come back to it later with some other fun things.

Could be fun to integrate ML somehow, but that will either be another
project or will just happen later on.