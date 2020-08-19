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

fig, axes = plt.subplots(2,2)


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

	# gyro x
	if(len(gyro_x_series) < HARD_LIMIT):
		gyro_x_series.append(gyro_x)
	else:
		gyro_x_series.append(gyro_x)
		gyro_x_series.remove(gyro_x_series[0])

	# gyro y
	if(len(gyro_y_series) < HARD_LIMIT):
		gyro_y_series.append(gyro_y)
	else:
		gyro_y_series.append(gyro_y)
		gyro_y_series.remove(gyro_y_series[0])

	# gyro z
	if(len(gyro_z_series) < HARD_LIMIT):
		gyro_z_series.append(gyro_z)
	else:
		gyro_z_series.append(gyro_z)
		gyro_z_series.remove(gyro_z_series[0])

	# #filtered data
	# filtered_accel_x_series = filter.LPF(accel_x_series)
	# filtered_accel_y_series = filter.LPF(accel_y_series)
	# filtered_accel_z_series = filter.LPF(accel_z_series)
	# filtered_gyro_x_series = filter.LPF(gyro_x_series)
	# filtered_gyro_y_series = filter.LPF(gyro_y_series)
	# filtered_gyro_z_series = filter.LPF(gyro_z_series)
	n = [i for i in range(0, len(accel_x_series))]

	#clearing stuff
	axes[0,0].clear()
	axes[0,1].clear()
	axes[1,0].clear()
	axes[1,1].clear()

	#plotting raw accel
	axes[0,0].plot(n, accel_x_series)
	axes[0,0].plot(n, accel_y_series)
	axes[0,0].plot(n, accel_z_series)
	axes[0,0].legend(['X', 'Y', 'Z'])
	axes[0,0].set_title("Raw Accel")

	#plotting raw gyro
	axes[0,1].plot(n, gyro_x_series)
	axes[0,1].plot(n, gyro_y_series)
	axes[0,1].plot(n, gyro_z_series)
	axes[0,1].legend(['X', 'Y', 'Z'])
	axes[0,1].set_title("Raw Gyro")

	# #plotting filtered accel
	# axes[1, 0].plot(n, filtered_accel_x_series)
	# axes[1, 0].plot(n, filtered_accel_y_series)
	# axes[1, 0].plot(n, filtered_accel_z_series)
	# axes[1, 0].legend(['X', 'Y', 'Z'])
	# axes[1, 0].set_title("Filtered Accel")

	# #plotting filtered gyro
	# axes[1,1].plot(n, filtered_gyro_x_series)
	# axes[1,1].plot(n, filtered_gyro_y_series)
	# axes[1,1].plot(n, filtered_gyro_z_series)
	# axes[1,1].legend(['X', 'Y', 'Z'])
	# axes[1,1].set_title("Filtered Gyro")

ani = animation.FuncAnimation(fig, animate, interval=1)
plt.show()