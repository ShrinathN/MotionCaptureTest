#!/bin/python3

from socket import *
import struct
import sys
import pandas as pd
import matplotlib.pyplot as plt

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

try:
	while True:
		data0 = s.recv(24)
		if len(data0) != 24:
			continue
		dataf = struct.unpack("6f", data0)
		accel_x = dataf[0]
		accel_y = dataf[1]
		accel_z = dataf[2]
		gyro_x = dataf[3]
		gyro_y = dataf[4]
		gyro_z = dataf[5]
		print("accel_x : " + str(accel_x))
		print("accel_y : " + str(accel_y))
		print("accel_z : " + str(accel_z))
		print("gyro_x : " + str(gyro_x))
		print("gyro_y : " + str(gyro_y))
		print("gyro_z : " + str(gyro_z))
		accel_x_ser.append(accel_x)
		accel_y_ser.append(accel_y)
		accel_z_ser.append(accel_z)
		gyro_x_ser.append(gyro_x)
		gyro_y_ser.append(gyro_y)
		gyro_z_ser.append(gyro_z)

except KeyboardInterrupt:
    df = pd.DataFrame({
		"accel_x": accel_x_ser,
		"accel_y": accel_x_ser,
		"accel_z": accel_x_ser,
		"gyro_x_ser" : gyro_x_ser,
		"gyro_y_ser" : gyro_y_ser,
		"gyro_z_ser" : gyro_z_ser
	})
    df.to_csv("out.csv")

n = [i for i in range(0, len(accel_x_ser))]

fig, axs = plt.subplots(2)

#accelerometer plot
axs[0].plot(n, accel_x_ser)
axs[0].plot(n, accel_y_ser)
axs[0].plot(n, accel_z_ser)
axs[0].legend(["X", "Y", "Z"])
axs[0].set_title('Accelerometer (m/s^2)')

axs[1].plot(n, gyro_x_ser)
axs[1].plot(n, gyro_y_ser)
axs[1].plot(n, gyro_z_ser)
axs[1].legend(["X", "Y", "Z"])
axs[1].set_title('Gyroscope (rad/s)')

plt.show()
