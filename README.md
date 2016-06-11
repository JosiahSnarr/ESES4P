ESE Semester 4 Project Software
===============================

The ESE Semester 4 Project Software is software to support the HCS12 embedded system controlling a small robot.
For more information on the hardware, see how I turned my Altium design into a physical board: http://imgur.com/gallery/lOScd/new

Modules
-------

* LCD (lcd)
* Servo Motor (servo)
* Accelerometer (accelerometer)
* DAC (within spi)
* Serial Communication (SCI)
* Stepper Motor (stepper)
* DC Motors (DCmotor)

Utilities
---------

* Basic Bit Operation Macros (macros)
* Timer Channels (timer)
* SPI (spi)

Notes:
------

The project is compiled using Freescale's CodeWarrior. The repository only contains modules, without supporting CodeWarrior libraries.
