# motor-sensor
A simple model of an automated emergency braking system using the [ultrasonic module HC-SR04](https://www.robotshop.com/ca/en/hc-sr04-ultrasonic-range-finder-tys.html?gclid=Cj0KCQiA0NfvBRCVARIsAO4930kOmOdwf8OKxRdgC7gpzQ4CmaPxtbPPZlcoTEany2mF2SZQzRMwBLEaAsoIEALw_wcB), a [TI (Texas Intrument) MSP430 16-bit micro-controller](http://www.ti.com/microcontrollers/msp430-ultra-low-power-mcus/overview.html), and an [Ivolador motor robot car chassis kit](https://www.dhgate.com/product/ivolador-motor-robot-car-chassis-kit-with/401347321.html#seo=WAP).

## Part Diagram

## Circuit Diagram 
![](https://github.com/rdesc/motor-sensor/blob/master/docs/circuitdiagram.png)

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
