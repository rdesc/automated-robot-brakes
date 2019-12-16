#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Feb 25 22:11:42 2018

@author: rodriguedeschaetzen
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

data = np.loadtxt("motor_data.txt")

x = data[:,0]
y = data[:,1]

def func(x, m, b):
    return m*x+b

plt.xlabel('Timestep (s)')
plt.ylabel('Distance (cm)')
plt.title('Distance of Nearest Obstacle as a Function of Time')
plt.grid(True)

plt.plot(x, y, color='b', label='data')

popt, pcov = curve_fit(func, x, y)
plt.plot(x, func(x, *popt), 'r-', label='fit: m=%5.3f, b=%5.3f' % tuple(popt))
plt.legend()

plt.savefig("motor_data_fit.pdf")

