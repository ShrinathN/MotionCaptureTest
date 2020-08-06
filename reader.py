#!/bin/python3

from socket import *
import struct
import sys
import pandas as pd
import matplotlib.pyplot as plt
import filter

accel_x_ser = []
accel_y_ser = []
accel_z_ser = []
gyro_x_ser = []
gyro_y_ser = []
gyro_z_ser = []

s = socket(AF_INET, SOCK_DGRAM, 0)
# s.setsockopt(SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 1)
s.bind(("0.0.0.0", 8080))
s.setblocking(True)

first_run = True

try:
	while(True):
		data0 = s.recv(24)
		if(len(data0) != 24):
			continue
		if(first_run):
			first_run = False
			print('Recording!')
		dataf = struct.unpack("6f", data0)
		accel_x = dataf[0]
		accel_y = dataf[1]
		accel_z = dataf[2]
		gyro_x = dataf[3]
		gyro_y = dataf[4]
		gyro_z = dataf[5]
		accel_x_ser.append(accel_x)
		accel_y_ser.append(accel_y)
		accel_z_ser.append(accel_z)
		gyro_x_ser.append(gyro_x)
		gyro_y_ser.append(gyro_y)
		gyro_z_ser.append(gyro_z)

except KeyboardInterrupt:
    df = pd.DataFrame({
		"accel_x": accel_x_ser,
		"accel_y": accel_y_ser,
		"accel_z": accel_z_ser,
		"gyro_x_ser" : gyro_x_ser,
		"gyro_y_ser" : gyro_y_ser,
		"gyro_z_ser" : gyro_z_ser
	})
    df.to_csv("out.csv")

n = [i for i in range(0, len(accel_x_ser))]

fig, axs = plt.subplots(2,2)

#accelerometer plot
axs[0,0].plot(n, accel_x_ser)
axs[0,0].plot(n, accel_y_ser)
axs[0,0].plot(n, accel_z_ser)
axs[0,0].legend(["X", "Y", "Z"])
axs[0,0].set_title('Accelerometer (m/s^2)')
#gyroscope plot
axs[1,0].plot(n, gyro_x_ser)
axs[1,0].plot(n, gyro_y_ser)
axs[1,0].plot(n, gyro_z_ser)
axs[1,0].legend(["X", "Y", "Z"])
axs[1,0].set_title('Gyroscope (rad/s)')

accel_x_ser_window_filtered = filter.window_filter(accel_x_ser)
accel_y_ser_window_filtered = filter.window_filter(accel_y_ser)
accel_z_ser_window_filtered = filter.window_filter(accel_z_ser)
gyro_x_ser_window_filtered = filter.window_filter(gyro_x_ser)
gyro_y_ser_window_filtered = filter.window_filter(gyro_y_ser)
gyro_z_ser_window_filtered = filter.window_filter(gyro_z_ser)

#window filter data
#accelerometer plot
axs[0,1].plot(n, accel_x_ser_window_filtered)
axs[0,1].plot(n, accel_y_ser_window_filtered)
axs[0,1].plot(n, accel_z_ser_window_filtered)
axs[0,1].legend(["X", "Y", "Z"])
axs[0,1].set_title('Accelerometer  window filtered (m/s^2)')
#gyroscope plot
axs[1,1].plot(n, gyro_x_ser_window_filtered)
axs[1,1].plot(n, gyro_y_ser_window_filtered)
axs[1,1].plot(n, gyro_z_ser_window_filtered)
axs[1,1].legend(["X", "Y", "Z"])
axs[1,1].set_title('Gyroscope window filtered (rad/s)')

plt.show()
