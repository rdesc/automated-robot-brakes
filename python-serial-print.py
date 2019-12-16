#!/usr/bin/python2.7
import serial # for serial port
import numpy as np # for arrays, numerical processing
from time import time 

port = "/dev/tty.uart-51FF41CE9615043B" #For Mac?


#start our program proper:
#open the serial port
try:
    ser = serial.Serial(port,2400,timeout = 0.050) 
    ser.baudrate=9600
# with timeout=0, read returns immediately, even if no data
except:
    print ("Opening serial port",port,"failed")
    print ("Edit program to point to the correct port.")
    print ("Hit enter to exit")
    raw_input()
    quit()

ser.flushInput()
start_time = time()
f = open("motor_data.txt", "w")

while(1): #loop forever
    data = ser.read(1) # look for a character from serial port - will wait for up to 50ms (specified above in timeout)
    if len(data) > 0: #was there a byte to read?
        print time()-start_time,ord(data)
        f.write(str(time()-start_time)+" "+str(ord(data))+"\n")



