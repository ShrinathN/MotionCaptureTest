#!/bin/python3

from socket import *
import struct
import sys
import pandas as pd
import matplotlib.pyplot as plt

xser = []
yser = []
zser = []


s = socket(AF_INET, SOCK_STREAM, 0)
s.setsockopt(SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 1)
s.setblocking(True)
s.connect((sys.argv[1], int(sys.argv[2])))

try:
	while(True):
		s.send(b'0')
		data0 = s.recv(12)
		if(len(data0) != 12):
			continue
		dataf = struct.unpack('3f', data0)
		x = dataf[0]
		y = dataf[1]
		z = dataf[2]
		print('x : ' + str(x))
		print('y : ' + str(y))
		print('z : ' + str(z))
		xser.append(x)
		yser.append(y)
		zser.append(z)
except KeyboardInterrupt:
	df = pd.DataFrame({
		'x' : xser,
		'y' : yser,
		'z' : zser
		})
	df.to_csv('out.csv')

n = [i for i in range(0, len(xser))]
plt.plot(n, xser)
plt.plot(n, yser)
plt.plot(n, zser)
plt.legend(['X', 'Y', 'Z'])
plt.show()
