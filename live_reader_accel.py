#!/bin/python3

from socket import *
import struct
import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import filter

HARD_LIMIT = 5

s = socket(AF_INET, SOCK_DGRAM, 0)
s.bind(('0.0.0.0', 8080))
s.setblocking(True)

accel_x_series = []
accel_y_series = []
accel_z_series = []
gyro_x_series = []
gyro_y_series = []
gyro_z_series = []

fig,axes = plt.subplots(1)

def animate(i):
	data = struct.unpack("6f",s.recv(24))
	accel_x = data[0]
	accel_y = data[1]
	accel_z = data[2]
	gyro_x = data[3]
	gyro_y = data[4]
	gyro_z = data[5]
	#adding data
	# accel x
	if(len(accel_x_series) < HARD_LIMIT):
		accel_x_series.append(accel_x)
	else:
		accel_x_series.append(accel_x)
		accel_x_series.remove(accel_x_series[0])
	# accel y
	if(len(accel_y_series) < HARD_LIMIT):
		accel_y_series.append(accel_y)
	else:
		accel_y_series.append(accel_y)
		accel_y_series.remove(accel_y_series[0])
	# accel z
	if(len(accel_z_series) < HARD_LIMIT):
		accel_z_series.append(accel_z)
	else:
		accel_z_series.append(accel_z)
		accel_z_series.remove(accel_z_series[0])

	
	n = [i for i in range(0, len(accel_x_series))]

	#clearing stuff
	axes.clear()

	#plotting raw accel
	axes.plot(n, accel_x_series)
	axes.plot(n, accel_y_series)
	axes.plot(n, accel_z_series)
	axes.legend(['X', 'Y', 'Z'])
	axes.set_title("Raw Accel")


ani = animation.FuncAnimation(fig, animate, interval=0)
plt.show()