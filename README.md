# motor-sensor
A simple model of an automated emergency braking system using the ultrasonic module HC-SR04, a TI (Texas Intrument) MSP430 16-bit micro-controller, and an Ivolador motor robot car chassis kit.

## Parts
A breadboard was attached to the front of the chassis along with the ultrasonic sensor and a piezoelectric buzzer. An external battery (5 V) was used to power the MSP430. A 6 V battery pack powers the drivers (V_CC2 - pin 8) in the h-bridge which powers the motors.

// TODO add diagram of parts

// TODO add pin connections

// TODO add schematic

## Setup
These steps are specific to mac

1. Install msp430 development tools 

brew install mspdebug
2. Download and install the MSP430LPCDC1.0.3b.zip driver

3. compile the source:
make
load it into the Launchpad:
mspdebug rf2500
prog main.elf
^D 
